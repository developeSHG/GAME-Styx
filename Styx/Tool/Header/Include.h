#ifndef Include_h__
#define Include_h__

/*  Client Headers  */
#include "Typedef.h"
#include "Macro.h"
#include "Function.h"
#include "Enum.h"
#include "Struct.h"
#include "Value.h"
#include "Pattern.h"

/*  Tool Header Info  */
enum TOOLID { TOOL_MESH, TOOL_NAVIMESH, TOOL_ANIMATION, TOOL_CAMERA, TOOL_LIGHT, TOOL_END };

enum TOOLSCENEID { TOOLSCENE_WORLD, TOOLSCENE_SPACE, TOOLSCENE_END };

enum FILLMODE { FILL_WIREFRAME, FILL_SOLID, FILL_END };

enum MESHTREE { STATIC_MESH, DYNAMIC_MESH, STATIC_OBJECT, DYNAMIC_OBJECT, MESHTREE_END };


#endif // Include_h__
