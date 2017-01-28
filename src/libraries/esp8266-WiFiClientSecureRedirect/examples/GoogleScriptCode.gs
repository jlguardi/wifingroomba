function doGet(e) {

  var cal = CalendarApp.getCalendarById('your.google.email@gmail.com');
  if (cal == undefined) {
    return ContentService.createTextOutput("no access to calendar");
  }

  const now = new Date();
  var start = new Date(); start.setHours(0, 0, 0);  // start at midnight
  const oneday = 24*3600000; // [msec]
  const stop = new Date(start.getTime() + 7 * oneday);
  Logger.log(start);
  Logger.log(stop);
  
  var events = cal.getEvents(start, stop);
  
  var str = '';
  for (var ii = 0; ii < events.length; ii++) {

    var event=events[ii];    
    var myStatus = event.getMyStatus();
    
    switch(myStatus) {
      case CalendarApp.GuestStatus.OWNER:
      case CalendarApp.GuestStatus.YES:
      case CalendarApp.GuestStatus.MAYBE:
        str += event.getStartTime() + '\t' +
               event.isAllDayEvent() + '\t' +
               event.getPopupReminders()[0] + '\t' +
               event.getTitle() +'\n';
        break;
      default:
        break;
    }
  }
  return ContentService.createTextOutput(str);
}