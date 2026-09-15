#ifndef AUDIO_INTERFACE_H
#define AUDIO_INTERFACE_H

#include "STD_Types.h"

#define NOTE_DO4     0U
#define NOTE_RE4     1U
#define NOTE_MI4     2U
#define NOTE_FA4     3U
#define NOTE_SOL4    4U
#define NOTE_LA4     5U
#define NOTE_SI4     6U

#define NOTE_DO5     7U
#define NOTE_RE5     8U
#define NOTE_MI5     9U
#define NOTE_FA5     10U
#define NOTE_SOL5    11U
#define NOTE_LA5     12U
#define NOTE_SI5     13U

#define AUDIO_NOTES_COUNT    14U

void AUDIO_Init(void);
void AUDIO_Play(void);
void AUDIO_Stop(void);
void AUDIO_PlayNote(u8 Copy_u8Note);

#endif /* AUDIO_INTERFACE_H */
