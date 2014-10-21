#ifndef _VAD_H_INCLUDED_
#define _VAD_H_INCLUDED_
int vad(char* pcmFN, float* sted, int sted_len, float max_piece_len, float sil_delimit_len);
#endif