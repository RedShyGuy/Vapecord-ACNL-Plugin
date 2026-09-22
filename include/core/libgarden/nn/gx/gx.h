#pragma once

#include <core/libgarden/GLES2/gl2.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum : GLenum
{
	NN_GX_CMDLIST_RUN_MODE = 0x0200,
	NN_GX_CMDLIST_IS_RUNNING = 0x0201,
	NN_GX_CMDLIST_USED_BUFSIZE = 0x0202,
	NN_GX_CMDLIST_USED_REQCOUNT = 0x0203,
	NN_GX_CMDLIST_MAX_BUFSIZE = 0x0204,
	NN_GX_CMDLIST_MAX_REQCOUNT = 0x0205,
	NN_GX_CMDLIST_TOP_BUFADDR = 0x0206,
	NN_GX_CMDLIST_BINDING = 0x0207,
	NN_GX_CMDLIST_CURRENT_BUFADDR = 0x0208,
	NN_GX_CMDLIST_RUN_BUFSIZE = 0x0209,
	NN_GX_CMDLIST_RUN_REQCOUNT = 0x020a,
	NN_GX_CMDLIST_TOP_REQADDR = 0x020b,
	NN_GX_CMDLIST_NEXT_REQTYPE = 0x020c,
	NN_GX_CMDLIST_NEXT_REQINFO = 0x020d,
	NN_GX_CMDLIST_HW_STATE = 0x020e,
	NN_GX_CMDLIST_GAS_UPDATE = 0x020f,

	NN_GX_CMDGEN_MODE_CONDITIONAL = 0x0800,
	NN_GX_CMDGEN_MODE_UNCONDITIONAL = 0x0801,
};

void nngxGenCmdlists(GLsizei count, GLuint* out);
void nngxCmdlistStorage(GLsizei bufSize, GLsizei reqCount);
void nngxGetCmdlistParameteri(GLenum param, GLint* out);
void nngxBindCmdlist(GLuint);
void nngxClearCmdlist();
void nngxStartSaveCmdlist();
void nngxStopCmdlistSave(GLuint* bufOffset, GLsizei* bufSize, GLuint* reqOffset, GLsizei* reqSize);
void nngxUseSavedCmdlist(GLuint cmdlist, GLuint bufOffset, GLsizei bufSize, GLuint reqOffset, GLsizei reqSize, GLbitfield state, GLboolean copy);

GLboolean nngxGetIsRunning(void);

void nngxSetCommandGenerationMode(GLenum);

void nngxUpdateState(GLbitfield);
void nngxValidateState(GLbitfield, GLboolean);

// __cb_current_command_buffer accessed via gardenex::GetCmdBuffer()

#ifdef __cplusplus
}
#endif
