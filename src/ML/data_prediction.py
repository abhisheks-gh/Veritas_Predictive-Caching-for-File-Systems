import pickle
import time
import datetime

# day dict
dictday = {
    'Mon' : 0,
    'Tue' : 1,
    'Wed' : 2,
    'Thu' : 3,
    'Fri' : 4,
    'Sat' : 5,
    'Sun' : 6
}

# load model
with open('model.pickle', 'rb') as f:
    loaded_model = pickle.load(f)

# running 
p = 1
while (p == 1):
    current_time = datetime.datetime.now()
    day = current_time.strftime('%A')[0:3]
    hour = int(current_time.strftime('%H'))
    print("Day Prefix:", day)
    print("Hour:", hour)
    if(hour == 23):
        # handling edge case wrt next day
        pred_hour = 0
        if(day == "Mon"):
            pred_day = "Tue" 
        elif(day == "Tue"):
            pred_day = "Wed"
        elif(day == "Wed"):
            pred_day = "Thu"
        elif(day == "Thu"):
            pred_day = "Fri"
        elif(day == "Fri"):
            pred_day = "Sat"
        elif(day == "Sat"):
            pred_day = "Sun"
        else:
            pred_day = "Mon"
    else: 
        pred_hour = hour + 1
        pred_day = day 
    
    # predicting for example input :
    predicted_file_path = loaded_model.predict([[pred_hour, dictday[pred_day]]])
    print(pred_day)
    print(pred_hour)
    print(f"Predicted File Path: {predicted_file_path}")
    time.sleep(10)
