#ifndef ESCAPE_ROOM_H
#define ESCAPE_ROOM_H
#include "set.h"
#include "list.h"

typedef struct escape_room_t *EscapeRoom;
EscapeRoom RoomCreate(int id, int price, int num_person, char* working_hours, 
                        int difficulty);
void RoomDestroy(void* room);
void* RoomCopy(void* room);
bool RoomHasBookings(EscapeRoom room);
int RoomGetId(EscapeRoom room);
int RoomGetPrice(EscapeRoom room);
bool RoomUserHasBookings(EscapeRoom room, char *email);
bool RoomAvailable(EscapeRoom room, int day, int hour);
#endif