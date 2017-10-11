
/*! The controller of dialogs with NPCs and objects */
class DialogWindow: DialogWidgetController
{
   string windowId;
   string selectorId;
   string ownerTextId;
   string portraitId;
   int selected;

   DialogWindow()
   {
      windowId = "dialogWindow";
      selectorId = "dialogOptions"; 
      ownerTextId = "dialogText";
      portraitId = "dialogPortrait";
      selected = -1;
   }

   string getWidgetFilename()
   {
      return "widgets/dialog.json";
   }

   /*! \return root widget identifier, for proper quit and delete */
   string getRootWidgetId()
   {
      return windowId;
   }

   void onEvent(int type, string id)
   {
      if((type == FARSO_EVENT_TEXTSELECTOR_OPTION_SELECTED) &&
         (id == selectorId))
      {
         selected = textSelectorGetSelected(selectorId);
      }
   }

   bool shouldQuit()
   {
      return !windowIsOpened(windowId);
   }

   void close()
   {
      rootWidgetRemove(windowId);
   }

   void setOwner(string name, string portrait)
   {
      widgetSetCaption(windowId, name);
      pictureSet(portraitId, portrait);
   }

   void clear()
   {
      textSelectorClear(selectorId);
      scrollTextClear(ownerTextId);
      selected = -1;
   }

   void addOption(string text, int index)
   {
      textSelectorAddOption(selectorId, text, index);
   }

   void selectOption(int index)
   {
      if(textSelectorHaveOption(selectorId, index))
      {
         selected = index;
      }
   }

   int getSelectedOption()
   {
      return selected;
   }

   void setOwnerText(string text)
   {
      scrollTextSet(ownerTextId, text);
   }

}

