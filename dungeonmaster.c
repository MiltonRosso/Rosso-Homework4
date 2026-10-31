// print out <Room Code>:<Room Name>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * 0. Create a repository.
 * 1. Open a file, that was not via command line.
 * 2. Be able to make *A* room.
 * 3. Be able to trim a string.
 * 4. How can we tell we're done reading a single room?
 * 5. How can we tell we're at "Room Name" line?
 * 6. How many rooms exist in the file?
 * 7. How can we trim the label of the line?
 * 8. How can we create a structure from the data we're reading in a loop?
 * 9. How can we print the information from a single room?
 * 10. How can we print the info for all rooms?
**/

typedef struct{
    char name[50];
    char code[3];
    char description[500];
} Room;

char *trim_whitespace(char *str);
void print_dungeon_room(Room dungeon_room[], int num_of_rooms);

int main(int argc, char **argv) {

    // Checks to see if there is more than one argument given.
    if(argc != 2) {
        printf("Your provided an inacurate number of arguments\n");
        printf("Example: dungeonRoomList_Large.txt");
        return 0;
    }

    // opens the first argument of the function
    FILE *dungeon_rooms = fopen(argv[1], "r");

    // Checks to see if the file was opened correctly
    if (dungeon_rooms == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Saves space in memory for the array of rooms
    int num_rooms = 0;
    int capacity = 2;
    Room *rooms = malloc(capacity * sizeof(Room));

    Room temp_room = {0};

    /* 
     * Tracks whether certain parts of the file have been read through
     * iteration.
    **/
    int has_name = 0;
    int has_code = 0;
    int has_description = 0;

    char buffer[1024];

    /*
     * Reads the file one line at a time, storing the name, code and
     * description of each room.
    **/
    while(fgets(buffer, sizeof(buffer), dungeon_rooms) != NULL) {
        
        // Finds the colon and seperates the lebel from the value
        char *value = strchr(buffer, ':');
        if(value == NULL) {
            continue;
        }
        *value = '\0';
        value++;

        // Removes the whitespace
        char *label = trim_whitespace(buffer);
        value = trim_whitespace(value);

        // Identifies the information
        if (strcmp(label, "Room Name") == 0) {
            if (strlen(value) >= sizeof(temp_room.name)) {
                printf("Room name too long.\n");
                free(rooms);
                fclose(dungeon_rooms);
                return 1;
            }

            strcpy(temp_room.name, value);
            has_name = 1;
        } else if (strcmp(label, "Room Code") == 0) {
            if (strlen(value) >= sizeof(temp_room.code)) {
                printf("Room code too long.\n");
                free(rooms);
                fclose(dungeon_rooms);
                return 1;
            }

            strcpy(temp_room.code, value);
            has_code = 1;
        } else if (strcmp(label, "Room Description") == 0) {
            if (strlen(value) >= sizeof(temp_room.description)) {
                printf("Room description too long.\n");
                free(rooms);
                fclose(dungeon_rooms);
                return 1;
            }

            strcpy(temp_room.description, value);
            has_description = 1;
        }

        // Checks whether the room is complete
        if (has_name && has_code && has_description) {
            if (num_rooms == capacity) {
                int new_capacity = capacity * 2;

                Room *temp = realloc(
                    rooms,
                    new_capacity * sizeof(Room)
                );

                if (temp == NULL) {
                    printf("Memory allocation failed.\n");
                    free(rooms);
                    fclose(dungeon_rooms);
                    return 1;
                }

                rooms = temp;
                capacity = new_capacity;
            }

            // Stores the completed room
            rooms[num_rooms] = temp_room;
            num_rooms++;

            // Resets the temporary room and trackers
            temp_room = (Room){0};

            has_name = 0;
            has_code = 0;
            has_description = 0;
        }
    }

    print_dungeon_room(rooms, num_rooms);

    free(rooms);
    fclose(dungeon_rooms);

    return 0;
}

// Removes whitespace from the beginning and end of a string
char *trim_whitespace(char *str) {

    while (isspace((unsigned char)*str)) {
        str++;
    }

    char *end = str + strlen(str);
    while (end > str && isspace((unsigned char)*(end - 1))) {
        end--;
    }

    *end = '\0';
    return str;
}

// Prints all stored rooms
void print_dungeon_room(Room dungeon_room[], int num_of_rooms) {

    for (int i = 0; i < num_of_rooms; i++) {
        printf("Room %s: %s\n",
               dungeon_room[i].code,
               dungeon_room[i].name);

        printf("Description: %s\n\n",
               dungeon_room[i].description);
    }
}