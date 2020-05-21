import matplotlib.pyplot as plt
import numpy as np
import matplotlib.axes as axes
from math import log
import csv
import sys
from datetime import datetime

chalmers_csv_list = []
grp_10_list = []

# set name for files by iteration from arguments
chalmers_file_name = "src/python/chalmers_" + sys.argv[1] + ".csv"
group10_file_name = "src/python/group10_" + sys.argv[1] + ".csv"

csvReader = csv.reader(open(chalmers_file_name), delimiter=";")
for row in csvReader:
    chalmers_csv_list.append(row)

csvReader = csv.reader(open(group10_file_name), delimiter=",")
for row in csvReader:
    grp_10_list.append(row)

# remove title of first column in chalmers csv
chalmers_csv_list.pop(0)

# remove title of first column in group_10 csv
grp_10_list.pop(0)

# store timestamp from group_10 csv in variable
TS_grp_10 = [i[0] for i in grp_10_list]
TS_grp_10_floats = []
# iterate over the list of timestamps to cast them to float in order to
# enable plotting of the values on the graph
for item in TS_grp_10:
    try:
        float(item)
        item = float(item)
        item = item/1000000
        TS_grp_10_floats.append(item)
    except ValueError:
        print(item)

# store groundsteering request from group_10 csv in variable
GSR_grp_10 = [i[1] for i in grp_10_list]

print(GSR_grp_10[0])
GSR_grp_10_floats = []

# iterate over the list of groundsteering requests to cast them to float
# in order to enable plotting of the values on the graph
for item in GSR_grp_10:
    try:
        float(item)
        item = float(item)
        GSR_grp_10_floats.append(item)
    except ValueError:
        print(item)

gsr_len = len(GSR_grp_10_floats)

ts_len = len(TS_grp_10_floats)
print(gsr_len)
print(ts_len)
#print(GSR_grp_10_floats)
print(TS_grp_10_floats)
print(TS_grp_10_floats[0])

print(TS_grp_10_floats[ts_len-1])
print(GSR_grp_10_floats[0])
print(GSR_grp_10_floats[gsr_len-1])


print(GSR_grp_10_floats[gsr_len-1] + 0.28)
print("---------------------------------------------------------------------")

#ground steering request
GSR = [i[6] for i in chalmers_csv_list]
#sample timesamp seconds
STSS = [i[4] for i in chalmers_csv_list]
#sample timestamp microseconds
STSMS = [i[5] for i in chalmers_csv_list]
for index, item in enumerate(GSR):
    GSR[index] = float(item)

array_len = len(STSS)

complete_arr = chalmers_csv_list

for i in range(array_len):
    microsec = STSMS[i]
    sec = STSS[i]
    merged = sec + '.'+ microsec
    complete_arr[i] = float(merged)

left = complete_arr[0]
right = complete_arr[array_len-1]


value = complete_arr[array_len-1] - complete_arr[0]
print(value)
plt.plot(complete_arr, GSR, 'r', label="Chalmers algorithm", linewidth=1)
plt.plot(TS_grp_10_floats, GSR_grp_10_floats, 'k', label="Group 10 algorithm", linewidth=1)
#plt.xticks(np.arange(TS_grp_10_floats[0], TS_grp_10_floats[ts_len-1] +1))

plt.ylabel('GroundSteeringRequest')
plt.xlabel('timestamp')
legend = plt.legend()


#-------------------- Acceptance testing ------
GSR_grp10 = []
TS_grp10 = []
# grp10_tuples = map(lambda x,y:(x,y),TS_grp_10_floats, GSR_grp_10_floats)
# chalmers_tuples = map(lambda x,y:(x,y),complete_arr, GSR)

numberOfTimestamps = 0
acceptedFrames = 0
startPos = []
endPos = []

for i in range(len(GSR)):

    # Get Chalmers GroundSteeringRequest of each tmestamp
    chalmers_GSR = GSR[i]
    #print('ChalmersGroundSteeringRequest: ' + str(chalmers_GSR))

    #Computing upper and lower limit. These differ depending on a negative or positive GSR.
    if chalmers_GSR >= 0:
        upperLimit =  chalmers_GSR * 1.5
        lowerLimit =  chalmers_GSR * 0.5
    else:
        upperLimit =  chalmers_GSR * 0.5
        lowerLimit =  chalmers_GSR * 1.5


    #To prevent out of bounds
    if len(GSR_grp_10_floats) > i:

        # Get Group 10 GroundSteeringRequest of each timestamp
        group10GSR = GSR_grp_10_floats[i]
        #print('Group 10 GroundSteeringRequest: ' + str(group10GSR))


        #if we pass the acceptance criteria
        if group10GSR <= upperLimit and group10GSR >= lowerLimit:

            #print('PASSED AT ITERATION ' + str(i))


            #Adding a frame to the array of accepted frames
            acceptedFrames += 1

            # Save this timestamp into an array, to be used later for plotting green areas on the chart
            if len(startPos) == len(endPos):
                #print('StartPos is' + str(complete_arr[i]))
                startPos.append(float(TS_grp_10_floats[i]))
                #print('1: Length of starpos is: ' + str(len(startPos)) + 'and lenght of EndPos is'  + str(len(endPos)))

        #If we don't pass the acceptance criteria
        else:
            #print('DID NOT PASS AT ITERATION ' + str(i))
            if len(startPos) > len(endPos):
                    #Saving the timestamp in an array, to be used later for plotting green areas on the chart
                endPos.append(float(TS_grp_10_floats[i - 1]))
                #print('EndPos is' + str(complete_arr[i]))
                #print('1: Length of starpos is: ' + str(len(startPos)) + 'and lenght of EndPos is'  + str(len(endPos)))



    numberOfTimestamps += 1

#Getting the percentage of how many frames we passed the acceptance criteria
percentagePassed = acceptedFrames / numberOfTimestamps

fullPercentage = percentagePassed * 100

fullPercentageString = str(fullPercentage)

if percentagePassed > 0.2:
    plt.title('Passed the Acceptance Criteria: ' + fullPercentageString + '%' )
else:
    plt.title('Did not meet the Acceptance Criteria: ' + fullPercentageString + '%')


#To prevent index out of bounds at line 187
if len(startPos) > len(endPos):
    lastIndex = len(TS_grp_10_floats) - 1
    closingAxisAtPoint = TS_grp_10_floats[lastIndex]
    endPos.append(float(closingAxisAtPoint))

#Filling the graph with green areas 
for i in range(len(startPos)):
    plt.axvspan(startPos[i], endPos[i], facecolor='#6de38c', alpha=0.5)

# get current time and time and stringify
now = datetime.now()
dt_string = now.strftime("%Y_%m_%d_%H_%M_%S")

# set name for acceptance png file 
acceptance_png = "src/python/acceptance_test_rec_" + sys.argv[1] + "_" + dt_string + ".png"

plt.savefig(acceptance_png, dpi=300, bbox_inches='tight')
