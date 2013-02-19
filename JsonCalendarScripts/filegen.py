import json
from icalendar import Calendar, Event, prop 
import datetime
import time
#import pytz

def dummycal():
    return "\
\
{\
    \"Alarms\": {\
        {\
          \"Time\": \"123456789\",\
          \"Name\": \"Meeting with bob\",\
          \"Location\":\"SO17 1BJ\",\
          \"Offset\": \"5700\"  \
        }\
    }\
\
}\
\
\
"

def getcal():
#    cal = Calendar.from_ical(open(ical,'rb').read())

    
    Table = {}
    Table['Alarms'] = []

#    FutureEvents = []
#    #print cal
#    for component in cal.walk('VEVENT'):
#        event = time.mktime(component.get('DTSTART').dt.timetuple())
#	now = time.mktime(datetime.datetime.now().timetuple())
#	if event >= now:
#            print component.get('SUMMARY')
#            Alarm={"Time":event , "Name":component.get('SUMMARY'), "Location":component.get('LOCATION'), "Offset":5700}
#	    FutureEvents.append(Alarm)


#    FutureEvents = sorted(FutureEvents, key=lambda k: k['Time'])

#    for x in range(0,6):
    utcnow = time.mktime(datetime.datetime.utcnow().timetuple())
#	midnight_utc = time.mktime(datetime.datetime.combine(utcnow.date()+datetime.timedelta(days=x), datetime.time(0)).timetuple())
#	midnight_tomorow = time.mktime(datetime.datetime.combine(utcnow.date()+datetime.timedelta(days=(x+1)), datetime.time(0)).timetuple())
#	print str(midnight_utc) + "->" + str(midnight_tomorow)
#
#	for event in FutureEvents:
#		if event["Time"]>=midnight_tomorow:
#			break
#		elif event["Time"]>=midnight_utc:
#			Table['Alarms'].append(event)
#			break
		


    Alarm={"Time":(utcnow+240) , "Name":"Cryptography Lecture", "Location":"B35/3005", "Offset":5700}
    Table['Alarms'].append(Alarm)

    Alarm={"Time":(utcnow+360) , "Name":"SUWS Meeting", "Location":"The Crown", "Offset":5700}
    Table['Alarms'].append(Alarm)

    Alarm={"Time":(utcnow+3600) , "Name":"Meeting with SUSU", "Location":"Concourse", "Offset":5700}
    Table['Alarms'].append(Alarm)

    Alarm={"Time":(utcnow+86400) , "Name":"Meeting with bob", "Location":"SO17 1BJ", "Offset":5700}
    Table['Alarms'].append(Alarm)





    print json.dumps(Table, indent=2)


getcal()
