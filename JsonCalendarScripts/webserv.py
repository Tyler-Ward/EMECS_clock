from bottle import route, run
import json
from icalendar import Calendar, Event, prop 
import datetime
import time
#import pytz

@route('/dummy.ics')
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

@route('/<ical>')
def getcal(ical):
    cal = Calendar.from_ical(open(ical,'rb').read())

    
    Table = {}
    Table['Alarms'] = []

    FutureEvents = []
    #print cal
    for component in cal.walk('VEVENT'):
        event = time.mktime(component.get('DTSTART').dt.timetuple())
	now = time.mktime(datetime.datetime.now().timetuple())
	if event >= now:
            print component.get('SUMMARY')
            Alarm={"Time":event , "Name":component.get('SUMMARY'), "Location":component.get('LOCATION'), "Offset":5700}
	    FutureEvents.append(Alarm)


    FutureEvents = sorted(FutureEvents, key=lambda k: k['Time'])

    for x in range(0,6):
	utcnow = datetime.datetime.utcnow()
	midnight_utc = time.mktime(datetime.datetime.combine(utcnow.date()+datetime.timedelta(days=x), datetime.time(0)).timetuple())
	midnight_tomorow = time.mktime(datetime.datetime.combine(utcnow.date()+datetime.timedelta(days=(x+1)), datetime.time(0)).timetuple())
	print str(midnight_utc) + "->" + str(midnight_tomorow)

	for event in FutureEvents:
		if event["Time"]>=midnight_tomorow:
			break
		elif event["Time"]>=midnight_utc:
			Table['Alarms'].append(event)
			break
		


        #Alarm={"Time":123456789 , "Name":"Meeting with bob", "Location":"SO17 1BJ", "Offset":5700}
	#Table['Alarms'].append(Alarm)


    return json.dumps(Table, indent=2)

run(host='localhost', port=8080)
run(host='10.42.0.23', port=8080)

