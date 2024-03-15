//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#pragma hdrstop
#include "app.cpp"

#pragma argsused
int main(int argc, char* argv[]){
   TApp app;

   // Init the app.
   app.Init(argc, argv);

   // Run it.
   app.Run();

   return EXIT_SUCCESS;
}//end main
