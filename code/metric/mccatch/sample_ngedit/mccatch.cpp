//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#pragma hdrstop
#include "app.cpp"
#include <ctime>

#pragma argsused
int main(int argc, char* argv[]){
   TApp app;

   // Init the app.
   app.Init(argc, argv);

   // int time_before = time(NULL);
   // Run it.
   app.Run();
   // int time_after = time(NULL);
   // int time_diff = time_after - time_before;
   // cout << time_diff << endl;

   return EXIT_SUCCESS;
}//end main
