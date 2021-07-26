#ifndef stateMachine_included
#define stateMachine_included

typedef unsigned char u_char;
typedef unsigned int u_int;

void toggle_red();
void toggle_red_dim();
void toggle_red_bright();
void toggle_red_off();
void red_dim(int level);
void toggle_green();
void toggle_green_dim();
void toggle_green_bright();
void toggle_green_off();
void green_dim(int level);
void toy_off();
void police_siren();
void state_reset();
void first_song();
void first_tune();
void update_loc();

#endif // included
