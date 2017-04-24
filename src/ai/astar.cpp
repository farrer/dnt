/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "astar.h"
#include "patternagent.h"
#include "../collision/collision.h"
#include "../rules/character.h"
#include "../core/game.h"
#include "../gui/briefing.h"
#include <kobold/log.h>

#define SEARCH_LIMIT   1000  /**< Max Nodes the AStar will search */
#define SEARCH_INTERVAL   2  /**< Interval of Nodes when AStar will sleep */
#define PC_SEARCH_FACTOR  4  /**< Playable Character factor to searchinterval */
#define MIN_CALL        200  /**< Minimun time interval to call search again */

#define STEP_FACTOR        20  /**< Factor to step search */
#define STEP_FACTOR_INDOOR 10  /**< Indor Factor to step search */
#define MAX_DIST_FACTOR     2  /**< Only visit nodes at max X times 
                                    distant to the goal than the current one */

using namespace DNT;

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
AStar::AStar(bool isPC)
{
   ownerPC = isPC;
   patt = new PatternAgent(true);
   patt->defineDestiny(0, 0);
   patt->defineStepSize(0);
   patt->defineOrientation(0);

   state = ASTAR_STATE_OTHER;
   callTimer.resume();
   
   opened = NULL;
   closed = NULL;

   walking = false;
   maxDist = 0;
}

/****************************************************************
 *                         clearSearch                          *
 ****************************************************************/
void AStar::clearSearch()
{
   /*  Clear Closed Nodes */
   if(closed)
   {
      delete(closed);
      closed = NULL;
   }

   /* Clear Opened Nodes */
   if(opened)
   {
      delete(opened);
      opened = NULL;
   }

   /* Clear Variables */
   curStepSize = 1;
   walking = false;
   maxDist = 0;
}

/****************************************************************
 *                         Destructor                           *
 ****************************************************************/
AStar::~AStar()
{
   /* Finish and wait running thread, if needed */
   if(isRunning())
   {
      endThread();
   }

   /* Clear Search Structs */
   clearSearch();

   /* Clear Patter Structs */
   delete patt;
}

/****************************************************************
 *                            findPath                          *
 ****************************************************************/
void AStar::findPath(Character* actor, float x, float z, float stepSize,
                     bool forceCall)
{
   /* Verify time variation */
   if( (callTimer.getMilliseconds() >= MIN_CALL) || (forceCall))
   {
      callTimer.reset();

      /* If is running a previous search, must end and wait the tread */
      if(isRunning())
      {
         endThread();
      }

      /* Verify, if at fight mode, if not already walking */
      if((Game::isAtFightMode()) && (walking))
      {
         /* Can't change the walk, so keep the current one */
         state = ASTAR_STATE_FOUND;
         return;
      }      

      /* Clear the current search (if one) */
      clearSearch();

      /* Clear the previous path */
      clear();

      /* Creates the new Search */
      if(opened == NULL)
      {
         opened = new ListAStar();
      }
      else
      {
         opened->clear();
      }
      if(closed == NULL)
      {
         closed = new ListAStar();
      }
      else
      {
         closed->clear();
      }

      state = ASTAR_STATE_RUNNING;

      /* Define Actor and Destiny */
      curActor = actor;
      destinyX = x;
      destinyZ = z;
      curStepSize = stepSize;

      /* Verify the distance to the target. If too near, no need to walk. */
      float actualX = actor->getModel()->getPosition().x;
      float actualZ = actor->getModel()->getPosition().z;
      dX = (x - actualX);
      dZ = (z - actualZ);
      float dist = Ogre::Math::Sqrt( (dX*dX) + (dZ*dZ) );

      /* Set maxDist */
      maxDist = dist*MAX_DIST_FACTOR;

      if(dist < 5)
      {
         /* too near */
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
         return;
      }

      /* Verify if the actor is alive (dead things can't walk, right?) */
      if(!actor->isAlive())
      {
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
         return;
      }

      if(Game::isAtFightMode())
      {
         if(!actor->getCanMove())
         {
             /* Can't move! */
             state = ASTAR_STATE_NOT_FOUND;
             clearSearch();
             return;
         }
      }

      /* Verify if can occupy the destiny position */
      if(!Collision::canOccupy(actor, Ogre::Vector3(destinyX, 0, destinyZ))) 
      {
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
         return;
      }

//TODO
#if 0
      /* Verify if the destiny is in map and not too far away */
      if( (destinyX < 0) || (destinyZ < 0) || 
          (destinyX >= actualMap->getSizeX()*actualMap->squareSize()) ||
          (destinyZ >= actualMap->getSizeZ()*actualMap->squareSize()) )
      {
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
         return;
      }
#endif

      /* Put initial node at search */
      heuristic = dX + dZ;
      opened->insert(actualX, actualZ, 0, heuristic, -1, -1);

      /* Start the search thread */
      createThread();
   }
   else
   {
      /* Note: Verify if isn't running, since sometimes come here
       * when already searching for the same path. */
      if( (!isRunning()) && (state != ASTAR_STATE_FOUND) )
      {
         /* Won't search, so not found */
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
      }
   }
}

/****************************************************************
 *                              clear                           *
 ****************************************************************/
void AStar::clear()
{
   if(isRunning())
   {
      endThread();
   }
   delete patt;
   patt = new PatternAgent(true);
   state = ASTAR_STATE_OTHER;
   walking = false;
}

/****************************************************************
 *                               step                           *
 ****************************************************************/
bool AStar::step()
{
   doCycle();
   if(state == ASTAR_STATE_NOT_FOUND)
   {
      Briefing::addText(220, 20, 220, gettext("A* could not find a path!"),
            true);
   }

   return state == ASTAR_STATE_FOUND || state == ASTAR_STATE_NOT_FOUND;
}

/****************************************************************
 *                             doCycle                          *
 ****************************************************************/
void AStar::doCycle()
{
   float newg = 0;                   /* new gone value */
   PointAStar* node, *node2, *node3; /* auxiliar nodes */
   float posX=0, posZ=0;             /* positions */
   int i, j;                         /* counters */
   bool directionChange;             /* direction verification */
   float curDist = 0.0f;             /* current distance to goal  */
   int searchInterval = SEARCH_INTERVAL;

   /* Verify if is searching, if not, no need to run */
   if(state != ASTAR_STATE_RUNNING)
   {
      return;
   }

   if(ownerPC)
   {
      /* Must search for more cycles */
      searchInterval *= PC_SEARCH_FACTOR;
   }

   /* Verify Lists */
   if( (opened == NULL) || (closed == NULL) )
   {
      state = ASTAR_STATE_OTHER;
      return;
   }

//TODO Outdoor maps
   /* Define Pass sizes */
   float pass;
#if 0
   if(actualMap->isOutdoor())
   {
      pass = curStepSize*STEP_FACTOR;
   }
   else
#endif
   {
      pass = curStepSize * STEP_FACTOR_INDOOR;
   }

   /* Set the colision variables */
   for(j = 0; j < searchInterval; j++)
   {
      /* Verify if the search still exists or if the limit was reached */
      if( (opened->isEmpty()) || 
          ((closed->size() + opened->size()) >= SEARCH_LIMIT) )
      {
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
         return;
      }

      /* Get better node from queue */
      node = opened->findLowest();
      if(!node)
      {
         state = ASTAR_STATE_NOT_FOUND;
         clearSearch();
         return;
      }

       /* Verify if arrived at destiny */
      if( (node->x >= destinyX - (pass)) &&
          (node->x <= destinyX + (pass)) && 
          (node->z >= destinyZ - (pass)) &&
          (node->z <= destinyZ + (pass)) )
      {
         /* create new path */
         delete patt;
         patt = new PatternAgent(true);
         patt->defineDestiny(node->x, node->z);
         patt->defineStepSize(curStepSize);
         patt->defineOrientation(curActor->getModel()->getOrientation());
         destinyX = node->x;
         destinyZ = node->z;

         /* Make the Founded path */
         while( (node != NULL) )
         {
            patt->addWayPointFirst(node->x, node->z);
            node = closed->find(node->parentX, node->parentZ);
         }
         patt->removeLinearWayPoints();
         patt->setOrigin(curActor->getModel()->getPosition().x, 
               curActor->getModel()->getPosition().z);
         patt->definePosition(curActor->getModel()->getPosition().x, 
               curActor->getModel()->getPosition().z);

         /* We're done */
         clearSearch();
         state = ASTAR_STATE_FOUND;
         walking = true;

         return;
      }

      /* Visit all Adjacents Positions */
      for(i=1; i<9; i++)
      {
         /* Get current direction  */
         directionChange = false;
         dX = node->x - node->parentX;
         dZ = node->z - node->parentZ;

         /* Get next potential node  */
         switch(i) 
         {
           case 1:
           {
              posX = node->x;
              posZ = node->z - pass;
              directionChange = !( (dX == 0) && (dZ < 0) ); 
           }
           break;
           case 2:
           {
              posX = node->x + pass;
              posZ = node->z - pass;
              directionChange = !( (dX > 0) && (dZ < 0) );
           }
           break;
           case 3:
           {
              posX = node->x + pass;
              posZ = node->z;
              directionChange = !( (dX > 0) && (dZ == 0) );
           }
           break;
           case 4:
           {
              posX = node->x + pass;
              posZ = node->z + pass;
              directionChange = !( (dX > 0) && (dZ > 0) );
           }
           break;
           case 5:
           {
              posX = node->x;
              posZ = node->z + pass;
              directionChange = !( (dX == 0) && (dZ > 0) );
           }
           break;
           case 6:
           {
              posX = node->x - pass;
              posZ = node->z + pass;
              directionChange = !( (dX <  0) && (dZ > 0) );
           }
           break;
           case 7:
           {
              posX = node->x - pass;
              posZ = node->z;
              directionChange = !( (dX <  0) && (dZ == 0) );
           }
           break;
           case 8:
           {
              posX = node->x - pass;
              posZ = node->z - pass;
              directionChange = !( (dX <  0) && (dZ < 0) );
           }
           break;
        }

        /* Calculate current distance to goal */
        dX = Ogre::Math::Abs(posX - destinyX);
        dZ = Ogre::Math::Abs(posZ - destinyZ);
        curDist = Ogre::Math::Sqrt((dX * dX) + (dZ * dZ)); 

        /* Only look at valid nodes and not so far away from the
         * destiny. */
        if( (curDist <= maxDist) &&  
            (Collision::canMove(curActor, Ogre::Vector3(posX, 0, posZ), 
                                Ogre::Vector3(node->x, 0, node->z))) )
        {
           /* New Gone is the current gone + distance to this one */
           newg = node->gone + 
              Ogre::Math::Sqrt((posX - node->x) * (posX - node->x) + 
                    (posZ - node->z) * (posZ - node->z));

           /* Apply direction change penalty, if needed  */
           if(directionChange)
           {
              newg += 2;
           }

           /* search it at closed */
           node2 = closed->find(posX, posZ);

           /* search it at opened */
           node3 = opened->find(posX, posZ);

           /* Heuristic calculation */
           heuristic = dX + dZ;

           /* If is in open or closed and n.g <= new g */
           if( ((node2 != NULL) && (node2->gone <= newg)) || 
               ((node3 != NULL) && (node3->gone <= newg)) )
           {
              //skip
           }
           else
           {
              /* Remove it from closed */
              if(node2 != NULL)
              {
                 closed->remove(node2);
              }

              /* If is at open list */
              if(node3)
              {
                 /* Just update it */
                 node3->parentX = node->x;
                 node3->parentZ = node->z;
                 node3->gone = newg;
                 node3->heuristic = heuristic;
              }
              else
              {
                 /* Insert it */
                 opened->insert(posX, posZ, newg, heuristic, node->x, node->z);
              }
           }
        }
      }

      /* Push node onto closed */
      closed->insert(node->x, node->z, node->gone, node->heuristic, 
                     node->parentX, node->parentZ);
      opened->remove(node);
   }
}

/****************************************************************
 *                       setOrientation                         *
 ****************************************************************/
void AStar::setOrientation(float ori)
{
   patt->defineOrientation(ori);
}

/****************************************************************
 *                       getNewPosition                         *
 ****************************************************************/
bool AStar::getNewPosition(float& posX, float& posZ, float& ori,
                           bool run, float runMultiplier)
{
   float pX=0, pZ=0;

   /* Get the pattern next position */
   if(patt->defineNextPosition(run, runMultiplier))
   {
      /* If Position not changed (with the same angle), the move ended! */
      patt->getPosition(pX, pZ);
      if( (pX == posX) && (pZ == posZ) && (ori == patt->orientationValue()) )
      {
         walking = false;
         return false;
      }

      /* Update Position */
      posX = pX;
      posZ = pZ;
      ori = patt->orientationValue();

      if(Game::isAtFightMode())
      {
         /* Verify if overflow the max normal walk */
         if( (patt->getTotalWalked() > curActor->getDisplacement()) &&
             ( !curActor->getCanAttack() ) )
         {
            /* Can't do a full move, so stop here */
            curActor->setCanMove(false);
            walking = false;
            return false;
         }

         /* Verify if overflow the max full walk */
         else if(patt->getTotalWalked() >= 2*curActor->getDisplacement())
         {
            curActor->setCanAttack(false);
            curActor->setCanMove(false);
            walking = false;
            return false;
         }

         /* Verify if arrived at destiny */
         else if( (posX == destinyX) && (posZ == destinyZ) )
         {
            /* Update the booleans */
            if(patt->getTotalWalked() > curActor->getDisplacement())
            {
               curActor->setCanAttack(false);
            }
            curActor->setCanMove(false);
            
            /* Stop the motion */
            walking = false;
            return false;
         }

         /* Still walking */
         walking = true;
         return true;
      }
      else
      {
         /* Only stop when arrive at destiny */
         walking = (posX != destinyX) || (posZ != destinyZ);
         return walking;
      }
   }
   walking = false;
   return false;
}

/****************************************************************
 *                            getDestiny                        *
 ****************************************************************/
void AStar::getDestiny(float& destX, float& destZ)
{
   destX = destinyX;
   destZ = destinyZ;
}

/****************************************************************
 *                             getState                         *
 ****************************************************************/
AStar::AStarState AStar::getState()
{
   if(!isRunning())
   {
      AStarState st = state;
      if(state != ASTAR_STATE_RUNNING)
      {
         /* Only return FOUND or NOT_FOUND once. */
         state = ASTAR_STATE_OTHER;
      }
      return st;
   }
   return ASTAR_STATE_RUNNING;
}

/****************************************************************************/
/****************************************************************************/
/*                            LIST STAR                                     */
/****************************************************************************/
/****************************************************************************/


/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
ListAStar::ListAStar()
{
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
ListAStar::~ListAStar()
{
   clearList();
}

/****************************************************************
 *                            insert                            *
 ****************************************************************/
PointAStar* ListAStar::insert(float x, float z, float gone, 
      float heuristic, float parentX, float parentZ)
{
   PointAStar* newWay = find(x,z);
   if(newWay != NULL)
   {
      /* To not insert duplicate points */
      if(gone < newWay->gone)
      {
         newWay->gone = gone;
         newWay->heuristic = heuristic;
         newWay->parentX = parentX;
         newWay->parentZ = parentZ;
      }
      return newWay;
   }

   /* Not found, create a new */
   newWay = new PointAStar();
   newWay->x = x;
   newWay->z = z;
   newWay->gone = gone;
   newWay->heuristic = heuristic;
   newWay->parentX = parentX;
   newWay->parentZ = parentZ;

   /* Really insert on list */
   Kobold::List::insert(newWay);

   return newWay;
}

/****************************************************************
 *                           remove                             *
 ****************************************************************/
void ListAStar::remove(PointAStar* node)
{
   if(node)
   {
      Kobold::List::remove(node);
   }
   else
   {
      Kobold::Log::add("liststar: Trying to remove a NULL node!");
   }
}

/****************************************************************
 *                            find                              *
 ****************************************************************/
PointAStar* ListAStar::find(float x, float z)
{
   int aux;
   PointAStar* tmp = static_cast<PointAStar*>(getFirst());
   for(aux = 0; aux < getTotal(); aux++)
   {
      if( (tmp->x >= x-0.1f) && 
          (tmp->x <= x+0.1f) &&
          (tmp->z >= z-0.1f) &&
          (tmp->z <= z+0.1f) )
      {
         return tmp;
      }
      tmp = static_cast<PointAStar*>(tmp->getNext());
   }
   return NULL;
}

/****************************************************************
 *                            findLowest                        *
 ****************************************************************/
PointAStar* ListAStar::findLowest()
{
   int aux;
   PointAStar* tmp = static_cast<PointAStar*>(getFirst());
   PointAStar* lowest = static_cast<PointAStar*>(getFirst());

   float val=0.0;
   float lowestVal;

   if(getTotal() <= 0)
   {
      return NULL;
   }

   lowestVal = lowest->gone + lowest->heuristic;
   
   tmp = static_cast<PointAStar*>(getFirst()->getNext());
   for(aux = 1; aux < getTotal(); aux++)
   {
      val = tmp->gone + tmp->heuristic; 
      if( val < lowestVal )
      {
         lowestVal = val;
         lowest = tmp;
      }
      tmp = static_cast<PointAStar*>(tmp->getNext());
   }
   return lowest;
}

