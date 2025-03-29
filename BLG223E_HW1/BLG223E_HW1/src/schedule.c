#include "schedule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isSlotAvailable(struct Exam* examList, int start, int end) {
    struct Exam* current = examList;
    while (current != NULL) {
        if (!(current->endTime <= start || current->startTime >= end)) {
            return false; // overlap 
        }
        current = current->next;
        }
    return true; // no overlap
}


// Create a new schedule with 7 days
// Creates a new schedule with 7 days, linking them in a circular list with the head pointing to Monday.
struct Schedule* CreateSchedule() {

    struct Schedule* schedule = (struct Schedule*)malloc(sizeof(struct Schedule));
    schedule->head = NULL; // Initially, no days in the schedule
    
    const char* dayNames[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    // Create the first day (Monday) and set it as the head
    struct Day* prevDay = (struct Day*)malloc(sizeof(struct Day));
    
    // Initialize the first day (Monday)
    strcpy(prevDay->dayName, dayNames[0]);
    prevDay->nextDay = NULL;
    prevDay->examList = NULL;
    schedule->head = prevDay; // Set the head to Monday

    // Create the remaining 6 days and link them
    for (int i = 1; i < 7; i++) {
        struct Day* newDay = (struct Day*)malloc(sizeof(struct Day));
        

        // Initialize the new day
        strncpy(newDay->dayName, dayNames[i], MAX_DAY_NAME_LEN - 1);
        newDay->dayName[MAX_DAY_NAME_LEN - 1] = '\0'; // Ensure null termination
        newDay->nextDay = NULL;
        newDay->examList = NULL;

        // Link the previous day to the new day
        prevDay->nextDay = newDay;
        prevDay = newDay;
    }

    // Make the last day (Sunday) point back to the head (Monday) to complete the circular list
    prevDay->nextDay = schedule->head;

    return schedule;
}

// Add an exam to a day in the schedule
// Adds a new exam to the specified day in the schedule in chronological order.
int AddExamToSchedule(struct Schedule* schedule, const char* day, int startTime, int endTime, const char* courseCode) {

    // Check if the exam duration is valid
    // Returns 3 if exam duration is longer than 3 hours, or if start/end time falls outside of allowed hours.
    // Print "Invalid exam.
    if( (endTime-startTime) > 3 || startTime < 8 || startTime > 17 || endTime < 9 || endTime > 20 ){
        printf("Invalid exam.\n");
        return 3;
    }

    struct Exam* newExam = CreateExam(startTime , endTime , courseCode);
    struct Day* dayPointer = schedule->head; //init to monday      
    
    //iterate to find wanted day
    while(strcmp(dayPointer->dayName, day) != 0) {
        dayPointer= dayPointer->nextDay;
    } //found the wanted day now go to the chronological exams list



    //new exam is the first exam of the day
    if( dayPointer->examList==NULL || (dayPointer->examList->startTime >= newExam->endTime)){ 
        newExam->next = dayPointer->examList;
        dayPointer->examList = newExam;
        printf("%s exam added to %s at time %d to %d without conflict.\n", courseCode, day, startTime, endTime);
        return 0;
        // Returns 0 if added to schedule on the desired day/time.
        // Print "[courseCode] exam added to [day] at time [startTime] to [endTime] without conflict."
    }

    struct Exam* currentExam = dayPointer->examList;
    
    while (currentExam->next != NULL && currentExam->next->startTime < startTime) {
        currentExam = currentExam->next;
    }

    if (currentExam->endTime <= startTime && (currentExam->next == NULL || currentExam->next->start_time >= endTime)) { //kendi saatine eklendi
        newExam->next = currentExam->next;
        currentExam->next = newExam;
        printf("%s exam added to %s at time %d to %d without conflict.\n", courseCode, day, startTime, endTime);
        return 0;     
        // Returns 0 if added to schedule on the desired day/time.
        // Print "[courseCode] exam added to [day] at time [startTime] to [endTime] without conflict."   
        
    } else { // look for the next available time slot
        // Returns 1 if added to schedule but on a later day/time.
        // Print "[courseCode] exam added to [day] at time [startTime] to [endTime] due to conflict."

        struct Day* tempDayPointer = dayPointer;
        int examDuration = endTime - startTime;
        int newStartTime = startTime;
        
    
        while (tempDayPointer != NULL) {
            struct Exam* tempExam = tempDayPointer->examList;
            newStartTime = 8; // day start at 8 

            while (newStartTime + examDuration <= 20) {
                if (isSlotAvailable(tempExam, newStartTime, newStartTime + examDuration)) {
                    newExam->startTime = newStartTime;
                    newExam->endTime = newStartTime + examDuration;
                    newExam->next = tempExam->next;
                    tempExam->next = newExam;
                    printf("%s exam added to %s at time %d to %d due to conflict.\n", courseCode, tempDayPointer->dayName, newStartTime, newStartTime + examDuration);
                    return 1;
                }
                newStartTime++; // change 1 hour and iterate
            }

            //if no slot open look at next day
            tempDayPointer = tempDayPointer->nextDay;
        }
    }
     
    // Returns 2 if schedule is completely full.
    // Print "Schedule full. Exam cannot be added."
    printf("Schedule full. Exam cannot be added.\n");
    free(newExam);
    return 2;
}

// Remove an exam from a specific day in the schedule
int RemoveExamFromSchedule(struct Schedule* schedule, const char* day, int startTime) {
    // Removes an exam from the specified day in the schedule based on the  start time.
    // Returns 0 if exam is found and removed. 
    // Print "Exam removed successfully."

    struct Day* dayPointer = schedule->head; //init to monday    
    
    //iterate to find wanted day
    while(strcmp(dayPointer->dayName, day) != 0) {
        dayPointer= dayPointer->nextDay;
    } //found the wanted day now go to the chronological exams list and iterate in it


    struct Exam* tempExam = dayPointer->examList ; 
    struct Exam* prevExam = NULL;

    // Iterate through the exams in the day's list
    while (tempExam != NULL) {
        if (tempExam->startTime == startTime) {
            // Exam found, remove it from the list
            if (prevExam == NULL) { // Removing the first exam in the list
                dayPointer->examList = tempExam->next;
            } else { // Removing an exam that is not the first
                prevExam->next = tempExam->next;
            }

            free(tempExam);
            printf("Exam removed successfully.\n");
            return 0;
        }
        prevExam = tempExam;
        tempExam = tempExam->next;
    }

    // No exam found with the specified start time
    printf("Exam could not be found.\n");
    return 1;


}

// Update an exam in the schedule
// Updates an existing exam, changing its time and/or day.
int UpdateExam(struct Schedule* schedule, const char* oldDay, int oldStartTime, const char* newDay, int newStartTime, int newEndTime) {
    
    // Returns 3 if new exam duration is longer than 3 hours, or if start/end time falls outside of allowed hours.
    // Print "Invalid exam.
    if((newEndTime-newStartTime)> 3 || newStartTime < 8 || newEndTime >20 || newStartTime > 17 || newEndTime < 9){
        printf("Invalid exam.");
        return 3;
    }

    struct Day* dayPointer = schedule->head; //init to monday    
    
    //iterate to find wanted day
    while(strcmp(dayPointer->dayName, oldDay) != 0) {
        dayPointer= dayPointer->nextDay;
    } //found the wanted day now go to the chronological exams list and iterate in it
    
    struct Exam* tempExam = dayPointer->examList ;  
    struct Exam* prevExam = NULL;

    while (tempExam != NULL && tempExam->startTime != oldStartTime) {
        prevExam = tempExam;
        tempExam = tempExam->next;
    }

    if (tempExam == NULL) {
        printf("Exam could not be found.\n");
        return 2;
    }

    struct Day* newDayPointer = schedule->head;
    while (strcmp(newDayPointer->dayName, newDay) != 0) {
        newDayPointer = newDayPointer->nextDay;
    }
    

    // Check if the slot is available in the new day's exam list
    struct Exam* currentExam = newDayPointer->examList;
    struct Exam* newPrevExam = NULL;
    while (currentExam != NULL) {
        if ((newStartTime >= currentExam->startTime && newStartTime < currentExam->endTime) ||
            (newEndTime > currentExam->startTime && newEndTime <= currentExam->endTime)) {
            printf("Update unsuccessful.\n"); // Time conflict
            return 1;
        }

        if (currentExam->startTime > newStartTime) break;

        newPrevExam = currentExam;
        currentExam = currentExam->next;
    }

    // Remove the exam from the old day
    if (prevExam == NULL) { // Exam is the first in the list
        dayPointer->examList = tempExam->next;
    } else { // Exam is in the middle or end of the list
        prevExam->next = tempExam->next;
    }

    // Update exam timing
    tempExam->startTime = newStartTime;
    tempExam->endTime = newEndTime;

    // Insert the updated exam in the correct position 
    if (newPrevExam == NULL) { // Insert to start of the list
        tempExam->next = newDayPointer->examList;
        newDayPointer->examList = tempExam;
    } else { // Insert to middle or end of the list
        tempExam->next = newPrevExam->next;
        newPrevExam->next = tempExam;
    }

    printf("Update successful.\n");
    return 0;   

      
}


// Clear all exams from a specific day and relocate them to other days
// Clears all exams from the specified day in the schedule.
int ClearDay(struct Schedule* schedule, const char* day) {

    

    struct Day* dayPointer = schedule->head; //DAY TO BE CLEARED    
    
    //iterate to find wanted day
    while(strcmp(dayPointer->dayName, day) != 0) {
        dayPointer= dayPointer->nextDay;
    };

    if(dayPointer->examList == NULL){
        // Returns 1 if the day was already empty when the function was called.
        // Print "[Day] is already clear."
        printf("%s is already clear.\n", day);
        return 1;    
    }

    struct Exam* examToMove = dayPointer->examList;
    struct Exam* nextExam;

    // try to relocate each exam
    while (examToMove != NULL) {
        int relocated = 0;
        struct Day* targetDay = schedule->head;

        // Find a new day with an available slot for the exam
        do {
            if (strcmp(targetDay->dayName, day) != 0 && // Don't relocate to the same day
                isSlotAvailable(targetDay->examList, examToMove->startTime, examToMove->endTime)) {                
                AddExamToSchedule(schedule, targetDay->dayName, examToMove->startTime, examToMove->endTime, examToMove->courseCode);
                relocated = 1;
                break;
            }
            targetDay = targetDay->nextDay;
        } while (targetDay != schedule->head);

        if (!relocated) {
            // Returns 2 if the day had exams, but the schedule is full, and the exams could not be relocated.
            // Print "Schedule full. Exams from [Day] could not be relocated.
            printf("Schedule full. Exams from %s could not be relocated.\n", day);
            return 2;
        }

        // Move to the next exam and free the current one
        nextExam = examToMove->next;
        free(examToMove);
        examToMove = nextExam;
    }

    // All exams relocated successfully
    dayPointer->examList = NULL;  // Clear the exam list
    printf("%s is cleared, exams relocated.\n", day);
    return 0;
    // Returns 0 if the day had exams, and all exams were relocate successfully.
    // Print "[Day] is cleared, exams relocated."
    

}



// Clear all exams and days from the schedule and deallocate memory
void DeleteSchedule(struct Schedule* schedule) {
    // Destructs the entire schedule by removing all exams from all days and deallocating memory.
    // Print "Schedule is cleared."
    struct Day* currentDay = schedule->head;
    struct Day* nextDay;

    // iterate over each day in the schedule
    do {
        struct Exam* currentExam = currentDay->examList;
        struct Exam* nextExam;

        // free all exams for the current day
        while (currentExam != NULL) {
            nextExam = currentExam->next;
            free(currentExam);
            currentExam = nextExam;
        }

        // move to the next day and free the current day
        nextDay = currentDay->nextDay;
        free(currentDay);
        currentDay = nextDay;
    } while (currentDay != schedule->head);

    // free the Schedule structure 
    free(schedule);

    printf("Schedule is cleared.\n");
}



// Read schedule from file
int ReadScheduleFromFile(struct Schedule* schedule, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    char dayName[20];
    int startTime, endTime;
    char courseCode[20];
    struct Day* currentDay = NULL;

    // reading each day name and its exams from file
    while (fscanf(file, "%s", dayName) != EOF) {
        currentDay = schedule->head;

        // find day
        while (strcmp(currentDay->dayName, dayName) != 0) {
            currentDay = currentDay->nextDay;
        }

        // read exams until reaching a blank line (which separates days)
        while (fscanf(file, "%d %d %s", &startTime, &endTime, courseCode) == 3) {
            AddExamToSchedule(schedule, dayName, startTime, endTime, courseCode);
        }

        // Clear the input buffer for next day
        fscanf(file, "%*[^\n]"); // This will skip the rest of the line.
    }

    fclose(file);
    return 0;
}

// Write schedule to file
int WriteScheduleToFile(struct Schedule* schedule, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return -1;
    }

    struct Day* dayPointer = schedule->head;

    do {
        fprintf(file, "%s\n", dayPointer->dayName);
        struct Exam* examPointer = dayPointer->examList;

        // write each exam in the days list
        while (examPointer != NULL) {
            fprintf(file, "%d %d %s\n", examPointer->startTime, examPointer->endTime, examPointer->courseCode);
            examPointer = examPointer->next;
        }

        fprintf(file, "\n");  // separate days with a newline
        dayPointer = dayPointer->nextDay;
    } while (dayPointer != schedule->head);

    fclose(file);
    return 0;
}
