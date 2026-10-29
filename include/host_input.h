#ifndef HOST_INPUT_H
#define HOST_INPUT_H

// Host build: input the DS doesn't have, for the game's code (modern/host/frontend.c).
#ifdef __cplusplus
extern "C" {
#endif
#ifdef HOST
// the circle pad (the optional feature, modern/host/host_features.h): 1 while it is on and pushed out of its dead zone,
// with its position: x right, y up, 105 a full push (the range of the 3DS circle pad code the feature follows)
int host_circle_pad(int *x, int *y);
#else
#define host_circle_pad(x, y) 0
#endif
#ifdef __cplusplus
}
#endif

#endif
