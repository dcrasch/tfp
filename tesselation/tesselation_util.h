#ifndef __TESSELATION_UTIL_H__
#define __TESSELATION_UTIL_H__

Boolean CheckROMVerGreaterThan(UInt16 major, UInt16 minor);

#ifndef vchrJogUp
#  define vchrJogUp             (0x1700)
#  define vchrJogDown           (0x1701)
#  define vchrJogPushedUp       (0x1703)
#  define vchrJogPushedDown     (0x1704)
#endif
#ifndef vchrJogDown
#  define vchrJogDown           (0x1701)
#endif


#endif
