void progStop() { progStop("undefined error"); }
void progStop(const char* infoError) {
  logln();
  systemLog("ERROR  ",infoError, 6, 3);
  systemLog("Program","Terminated", 6, 3);
  systemLog("RESET  ","Press Button to Reboot ",6,3);
  logln();
  chime(false);
  
}