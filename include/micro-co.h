/*
 * This code is under public domain (CC0)
 * <https://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * To the extent possible under law, dearblue has waived all copyright
 * and related or neighboring rights to this work.
 *
 *     dearblue <dearblue@users.noreply.github.com>
 */

#ifndef MICRO_CO_H__
#define MICRO_CO_H__ 1

typedef struct co_state *co_state_t;

#define CO_INIT 0


#if defined(MICRO_CO_FORCE_USE_LABEL_AS_VALUE) || !defined(MICRO_CO_USE_SWITCH) && (defined(__GNUC__) || defined(__clang__))
#   define CO_SET_LABEL(STATE, LABEL) do { *STATE = (co_state_t)&&LABEL; } while (0)
#   define CO_JUMP_TO(STATE) do { goto **(void **)STATE; } while (0)

#elif !defined(MICRO_CO_USE_SWITCH) && (defined(__MSVC__) && (defined(__i386__) || defined(__x86_64__)))
#   warn    NOT TESTED
#   define CO_SET_LABEL(STATE, LABEL) do { __asm { mov [STATE], offset LABEL }; } while (0)
#   define CO_JUMP_TO(STATE) do { __asm { jmp [STATE] }; } while (0)

#else
#   ifndef MICRO_CO_USE_SWITCH
#       define MICRO_CO_USE_SWITCH 1
#   endif

#endif


#ifdef MICRO_CO_USE_SWITCH

/* switch 文による実装 */

#define CO_LABEL__ __LINE__

static inline unsigned int *_co_cast_context(co_state_t *co) { return (unsigned int *)co; }

#define co_begin(S)                                         \
    do {                                                    \
        unsigned int *_co_entity_ = _co_cast_context(S);    \
                                                            \
        if (*_co_entity_ == 0) {                            \
            *_co_entity_ = CO_LABEL__;                      \
        }                                                   \
                                                            \
        switch (*_co_entity_) {                             \
        do {                                                \
        case CO_LABEL__:;                                   \
        } while (0)                                         \

#define co_yield(RET)                                       \
        do {                                                \
            *_co_entity_ = CO_LABEL__;                      \
            return RET;                                     \
        case CO_LABEL__:;                                   \
        } while (0)                                         \

#define co_checkpoint()                                     \
        do {                                                \
            *_co_entity_ = CO_LABEL__;                      \
        case CO_LABEL__:;                                   \
        } while (0)                                         \

#define co_end()                                            \
        do {                                                \
            *_co_entity_ = CO_LABEL__;                      \
        case CO_LABEL__:;                                   \
            break;                                          \
        } while (0);                                        \
        }                                                   \
    } while (0)                                             \

#else /* MICRO_CO_USE_SWITCH */

/* gcc の label-as-value を用いた実装 */

#define CO_LABEL_2_(X) co_label_ ## X ## _
#define CO_LABEL_1_(X) CO_LABEL_2_(X)
#define CO_LABEL__ CO_LABEL_1_(__LINE__)

static inline void **_co_cast_context(co_state_t *co) { return (void **)co; }

#define co_begin(S)                                         \
    do {                                                    \
        void **_co_entity_ = _co_cast_context(S);           \
                                                            \
        do {                                                \
            if (*_co_entity_) {                             \
                CO_JUMP_TO(_co_entity_);                    \
            }                                               \
        } while (0)                                         \

#define co_yield(RET)                                       \
        do {                                                \
            CO_SET_LABEL(_co_entity_, CO_LABEL__);          \
            return RET;                                     \
            CO_LABEL__:;                                    \
        } while (0)                                         \

#define co_checkpoint()                                     \
        do {                                                \
            CO_SET_LABEL(_co_entity_, CO_LABEL__);          \
            CO_LABEL__:;                                    \
        } while (0)                                         \

#define co_end()                                            \
        do {                                                \
            CO_SET_LABEL(_co_entity_, CO_LABEL__);          \
            CO_LABEL__:;                                    \
        } while (0);                                        \
    } while (0)                                             \

#endif /* MICRO_CO_USE_SWITCH */

#endif /* MICRO_CO_H__ */
