/* Macros para depuracion:

   Defina GlobalError UN lugar de su programa con el comportamiento deseado
*/

#ifndef DEFINE_CONFIRM_H
#define DEFINE_CONFIRM_H

extern void GlobalError(char* archivo, int linea);

#define CONFIRM_TRUE(v, retval) \
    if (!(v)) { \
        GlobalError(__FILE__,__LINE__); \
        return (retval); \
    }

#define CONFIRM_NOTNULL(v, retval) \
    if (!(v)) { \
        GlobalError(__FILE__,__LINE__); \
        return (retval); \
    }

#define CONFIRM_RETURN(v) \
    if (!(v)) { \
        GlobalError(__FILE__,__LINE__); \
        return; \
    }

#define CONFIRM_GOTO(v, label) \
    if (!(v)) { \
        GlobalError(__FILE__,__LINE__); \
        goto label; \
    }
    
#endif    
