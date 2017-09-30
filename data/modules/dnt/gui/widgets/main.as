/* Main Screen implementation */
class MainWidget : WidgetController
{
   MainWidget()
   {
   }

   string getWidgetFilename()
   {
      return "widgets/main.json";
   }

   void onEvent(int type, string id)
   {
   }

   bool shouldQuit()
   {
      return false;
   }
}

