#ifndef ESCAPE_ROOM_H
#define ESCAPE_ROOM_H
#include "set.h"
#include "Booking.h"
#include "list.h"

typedef struct escape_room_t *EscapeRoom;
typedef enum {
    ROOM_SUCCESS,
    ROOM_OUT_OF_MEMORY,
    ROOM_INVALID_PARAMETER,
} RoomErr;

EscapeRoom RoomCreate(int id, int price, int num_person, char* working_hours, 
                        int difficulty);
void RoomDestroy(void* room);
void* RoomCopy(void* room);
bool RoomHasBookings(EscapeRoom room);
int RoomGetId(EscapeRoom room);
int RoomGetPrice(EscapeRoom room);
bool RoomUserHasBookings(EscapeRoom room, char *email, int hour, int day);
bool RoomAvailable(EscapeRoom room, int day, int hour);
RoomErr RoomAddBooking(EscapeRoom room, Booking booking);
int RoomGetScore(EscapeRoom room, int level, int num_ppl);
char* RoomGetClosestAvailable(EscapeRoom room);
int RoomCompare(void* room1, void* room2);
List RoomGetTodayList(EscapeRoom room);
#endif