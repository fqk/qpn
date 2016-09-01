/*****************************************************************************
* Model: game.qm
* File:  ./tunnel.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::tunnel.c} ...........................................................*/
#include "qpn.h"
#include "game.h"
#include "bsp.h"

Q_DEFINE_THIS_MODULE("tunnel")

/* local objects -----------------------------------------------------------*/
/*${AOs::Tunnel} ...........................................................*/
typedef struct Tunnel {
/* protected: */
    QMActive super;

/* private: */
    QMsm * mines[GAME_MINES_MAX];
    QMsm * mine1_pool[GAME_MINES_MAX];
    QMsm * mine2_pool[GAME_MINES_MAX];
    uint8_t blink_ctr;
    uint8_t last_mine_x;
    uint8_t last_mine_y;
    uint8_t wall_thickness_top;
    uint8_t wall_thickness_bottom;
    uint8_t wall_gap;
} Tunnel;

/* private: */
static void Tunnel_advance(Tunnel * const me);
static void Tunnel_plantMine(Tunnel * const me);
static void Tunnel_dispatchToAllMines(Tunnel * const me, QSignal sig, QParam par);

/* protected: */
static QState Tunnel_initial(Tunnel * const me);
static QState Tunnel_active  (Tunnel * const me);
static QMState const Tunnel_active_s = {
    (QMState const *)0, /* superstate (top) */
    Q_STATE_CAST(&Tunnel_active),
    Q_ACTION_CAST(0), /* no entry action */
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_show_logo  (Tunnel * const me);
static QState Tunnel_show_logo_e(Tunnel * const me);
static QState Tunnel_show_logo_x(Tunnel * const me);
static QMState const Tunnel_show_logo_s = {
    &Tunnel_active_s, /* superstate */
    Q_STATE_CAST(&Tunnel_show_logo),
    Q_ACTION_CAST(&Tunnel_show_logo_e),
    Q_ACTION_CAST(&Tunnel_show_logo_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_demo  (Tunnel * const me);
static QState Tunnel_demo_e(Tunnel * const me);
static QState Tunnel_demo_x(Tunnel * const me);
static QMState const Tunnel_demo_s = {
    &Tunnel_active_s, /* superstate */
    Q_STATE_CAST(&Tunnel_demo),
    Q_ACTION_CAST(&Tunnel_demo_e),
    Q_ACTION_CAST(&Tunnel_demo_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_playing  (Tunnel * const me);
static QState Tunnel_playing_e(Tunnel * const me);
static QState Tunnel_playing_x(Tunnel * const me);
static QMState const Tunnel_playing_s = {
    &Tunnel_active_s, /* superstate */
    Q_STATE_CAST(&Tunnel_playing),
    Q_ACTION_CAST(&Tunnel_playing_e),
    Q_ACTION_CAST(&Tunnel_playing_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_game_over  (Tunnel * const me);
static QState Tunnel_game_over_e(Tunnel * const me);
static QState Tunnel_game_over_x(Tunnel * const me);
static QMState const Tunnel_game_over_s = {
    &Tunnel_active_s, /* superstate */
    Q_STATE_CAST(&Tunnel_game_over),
    Q_ACTION_CAST(&Tunnel_game_over_e),
    Q_ACTION_CAST(&Tunnel_game_over_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_screen_saver  (Tunnel * const me);
static QState Tunnel_screen_saver_i(Tunnel * const me);
static QMState const Tunnel_screen_saver_s = {
    &Tunnel_active_s, /* superstate */
    Q_STATE_CAST(&Tunnel_screen_saver),
    Q_ACTION_CAST(0), /* no entry action */
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(&Tunnel_screen_saver_i)
};
static QState Tunnel_screen_saver_hide  (Tunnel * const me);
static QState Tunnel_screen_saver_hide_e(Tunnel * const me);
static QState Tunnel_screen_saver_hide_x(Tunnel * const me);
static QMState const Tunnel_screen_saver_hide_s = {
    &Tunnel_screen_saver_s, /* superstate */
    Q_STATE_CAST(&Tunnel_screen_saver_hide),
    Q_ACTION_CAST(&Tunnel_screen_saver_hide_e),
    Q_ACTION_CAST(&Tunnel_screen_saver_hide_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_screen_saver_show  (Tunnel * const me);
static QState Tunnel_screen_saver_show_e(Tunnel * const me);
static QState Tunnel_screen_saver_show_x(Tunnel * const me);
static QMState const Tunnel_screen_saver_show_s = {
    &Tunnel_screen_saver_s, /* superstate */
    Q_STATE_CAST(&Tunnel_screen_saver_show),
    Q_ACTION_CAST(&Tunnel_screen_saver_show_e),
    Q_ACTION_CAST(&Tunnel_screen_saver_show_x),
    Q_ACTION_CAST(0)  /* no intitial tran. */
};
static QState Tunnel_final  (Tunnel * const me);
static QState Tunnel_final_e(Tunnel * const me);
static QMState const Tunnel_final_s = {
    (QMState const *)0, /* superstate (top) */
    Q_STATE_CAST(&Tunnel_final),
    Q_ACTION_CAST(&Tunnel_final_e),
    Q_ACTION_CAST(0), /* no exit action */
    Q_ACTION_CAST(0)  /* no intitial tran. */
};


/* global objects ----------------------------------------------------------*/
Tunnel AO_Tunnel;

/* Active object definition ================================================*/
/*${AOs::Tunnel_ctor} ......................................................*/
void Tunnel_ctor(void) {
    uint8_t n;
    Tunnel *me = &AO_Tunnel;
    QMActive_ctor(&me->super, Q_STATE_CAST(&Tunnel_initial));
    for (n = 0; n < GAME_MINES_MAX; ++n) {
        me->mine1_pool[n] = Mine1_ctor(n); /* instantiate Mine1 in the pool */
        me->mine2_pool[n] = Mine2_ctor(n); /* instantiate Mine2 in the pool */
        me->mines[n] = (QMsm *)0;                     /* mine 'n' is unused */
    }
    me->last_mine_x = 0;   /* the last mine at the right edge of the tunnel */
    me->last_mine_y = 0;
}
/*${AOs::Tunnel} ...........................................................*/
/*${AOs::Tunnel::advance} ..................................................*/
static void Tunnel_advance(Tunnel * const me) {
    uint32_t rnd;

    rnd = (BSP_random() & 0xFFU);

    /* reduce the top wall thickness 18.75% of the time */
    if ((rnd < 48U) && (me->wall_thickness_top > 0U)) {
        --me->wall_thickness_top;
    }

    /* reduce the bottom wall thickness 18.75% of the time */
    if ((rnd > 208U) && (me->wall_thickness_bottom > 0U)) {
        --me->wall_thickness_bottom;
    }

    rnd = (BSP_random() & 0xFFU);

    /* grow the bottom wall thickness 19.14% of the time */
    if ((rnd < 49U)
        && ((GAME_TUNNEL_HEIGHT
             - me->wall_thickness_top
             - me->wall_thickness_bottom) > me->wall_gap))
    {
        ++me->wall_thickness_bottom;
    }

    /* grow the top wall thickness 19.14% of the time */
    if ((rnd > 207U)
        && ((GAME_TUNNEL_HEIGHT
             - me->wall_thickness_top
             - me->wall_thickness_bottom) > me->wall_gap))
    {
        ++me->wall_thickness_top;
    }

    /* advance the Tunnel by 1 game step to the left
    * and copy the Tunnel layer to the main frame buffer
    */
    BSP_advanceWalls(me->wall_thickness_top, me->wall_thickness_bottom);
}
/*${AOs::Tunnel::plantMine} ................................................*/
static void Tunnel_plantMine(Tunnel * const me) {
    uint32_t rnd = (BSP_random() & 0xFFU);

    if (me->last_mine_x > 0U) {
        --me->last_mine_x; /* shift the last Mine 1 position to the left */
    }
    /* last mine far enough? */
    if ((me->last_mine_x + GAME_MINES_DIST_MIN < GAME_TUNNEL_WIDTH)
        && (rnd < 8U)) /* place the mines only 5% of the time */
    {
        uint8_t n;
        for (n = 0U; n < Q_DIM(me->mines); ++n) { /*look for disabled mines */
            if (me->mines[n] == (QMsm *)0) {
                break;
            }
        }
        if (n < Q_DIM(me->mines)) { /* a disabled Mine found? */
            rnd = (BSP_random() & 0xFFFFU);

            if ((rnd & 1U) == 0U) { /* choose the type of the mine */
                me->mines[n] = me->mine1_pool[n];
            }
            else {
                me->mines[n] = me->mine2_pool[n];
            }

            /* new Mine is planted by the end of the tunnel */
            me->last_mine_x = GAME_TUNNEL_WIDTH - 8U;

            /* choose a random y-position for the Mine in the Tunnel */
            rnd %= (GAME_TUNNEL_HEIGHT
                    - me->wall_thickness_top
                    - me->wall_thickness_bottom - 4U);
            me->last_mine_y = (uint8_t)(me->wall_thickness_top + 2U + rnd);

            Q_SIG(me->mines[n]) = MINE_PLANT_SIG;
            Q_PAR(me->mines[n]) = (me->last_mine_x | (me->last_mine_y << 8));
            QMSM_DISPATCH(me->mines[n]); /* direct dispatch */
        }
    }
}
/*${AOs::Tunnel::dispatchToAllMines} .......................................*/
static void Tunnel_dispatchToAllMines(Tunnel * const me, QSignal sig, QParam par) {
    uint8_t n;

    for (n = 0; n < GAME_MINES_MAX; ++n) {
        if (me->mines[n] != (QMsm *)0) { /* is the mine used? */
            Q_SIG(me->mines[n]) = sig;
            Q_PAR(me->mines[n]) = par;
            QMSM_DISPATCH(me->mines[n]);
        }
    }
}
/*${AOs::Tunnel::SM} .......................................................*/
static QState Tunnel_initial(Tunnel * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { /* transition-action table */
        &Tunnel_show_logo_s, /* target state */
        {
            Q_ACTION_CAST(&Tunnel_show_logo_e), /* entry */
            Q_ACTION_CAST(0) /* zero terminator */
        }
    };
    /* ${AOs::Tunnel::SM::initial} */
    uint8_t n;
    for (n = 0; n < GAME_MINES_MAX; ++n) {
        QMSM_INIT(me->mine1_pool[n]); /* initial tran. for Mine1 */
        QMSM_INIT(me->mine2_pool[n]); /* initial tran. for Mine2 */
    }
    BSP_randomSeed(1234U); /* seed the pseudo-random generator */
    return QM_TRAN_INIT(&tatbl_);
}
/*${AOs::Tunnel::SM::active} ...............................................*/
/* ${AOs::Tunnel::SM::active} */
static QState Tunnel_active(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::MINE_DISABLED} */
        case MINE_DISABLED_SIG: {
            Q_ASSERT((Q_PAR(me) < GAME_MINES_MAX)
                     && (me->mines[Q_PAR(me)] != (QMsm *)0));
            me->mines[Q_PAR(me)] = (QMsm *)0;
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::PLAYER_QUIT} */
        case PLAYER_QUIT_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { /* transition-action table */
                &Tunnel_final_s, /* target state */
                {
                    Q_ACTION_CAST(&Tunnel_final_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::active::show_logo} ....................................*/
/* ${AOs::Tunnel::SM::active::show_logo} */
static QState Tunnel_show_logo_e(Tunnel * const me) {
    QActive_armX(&me->super, 0U,
        BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U); /* periodic */
    me->blink_ctr = 10U;
    BSP_paintString(24U, (GAME_TUNNEL_HEIGHT / 2U) - 8U, "Quantum LeAps");
    BSP_paintString(16U, (GAME_TUNNEL_HEIGHT / 2U) + 0U, "state-machine.com");

    BSP_paintString(1U, GAME_TUNNEL_HEIGHT - 18U, "Fire missile: BTN0");
    BSP_paintString(1U, GAME_TUNNEL_HEIGHT - 10U, "Fly ship up:  BTN1");

    BSP_updateScreen();
    return QM_ENTRY(&Tunnel_show_logo_s);
}
/* ${AOs::Tunnel::SM::active::show_logo} */
static QState Tunnel_show_logo_x(Tunnel * const me) {
    QActive_disarmX(&me->super, 0U);
    return QM_EXIT(&Tunnel_show_logo_s);
}
/* ${AOs::Tunnel::SM::active::show_logo} */
static QState Tunnel_show_logo(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::show_logo::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            --me->blink_ctr; /* toggle the blink couner */
            /* ${AOs::Tunnel::SM::active::show_logo::Q_TIMEOUT::[me->blink_ctr==0U]} */
            if (me->blink_ctr == 0U) {
                static struct {
                    QMState const *target;
                    QActionHandler act[3];
                } const tatbl_ = { /* transition-action table */
                    &Tunnel_demo_s, /* target state */
                    {
                        Q_ACTION_CAST(&Tunnel_show_logo_x), /* exit */
                        Q_ACTION_CAST(&Tunnel_demo_e), /* entry */
                        Q_ACTION_CAST(0) /* zero terminator */
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            /* ${AOs::Tunnel::SM::active::show_logo::Q_TIMEOUT::[else]} */
            else {
                /* ${AOs::Tunnel::SM::active::show_logo::Q_TIMEOUT::[else]::[(me->blink_ctr&1U)!=0U]} */
                if ((me->blink_ctr & 1U) != 0U) {
                    BSP_paintString(24U + 8U*6U, (GAME_TUNNEL_HEIGHT / 2U) - 8U,
                                    "LeAps");
                    BSP_updateScreen();
                    status_ = QM_HANDLED();
                }
                /* ${AOs::Tunnel::SM::active::show_logo::Q_TIMEOUT::[else]::[else]} */
                else {
                    BSP_paintString(24U + 8U*6U, (GAME_TUNNEL_HEIGHT / 2U) - 8U,
                                    "LeaPs");
                    BSP_updateScreen();
                    status_ = QM_HANDLED();
                }
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    (void)me; /* avoid compiler warning in case 'me' is not used */
    return status_;
}
/*${AOs::Tunnel::SM::active::demo} .........................................*/
/* ${AOs::Tunnel::SM::active::demo} */
static QState Tunnel_demo_e(Tunnel * const me) {
    me->last_mine_x = 0U; /* last mine at right edge of the tunnel */
    me->last_mine_y = 0U;
    /* set the tunnel properties... */
    me->wall_thickness_top = 0U;
    me->wall_thickness_bottom = 0U;
    me->wall_gap = GAME_WALLS_GAP_Y;

    /* clear the tunnel walls */
    BSP_clearWalls();

    QActive_armX(&me->super, 0U,
        BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U); /* periodic */
    me->blink_ctr = 20U*2U; /* 20s timeout total */
    return QM_ENTRY(&Tunnel_demo_s);
}
/* ${AOs::Tunnel::SM::active::demo} */
static QState Tunnel_demo_x(Tunnel * const me) {
    QActive_disarmX(&me->super, 0U);
    return QM_EXIT(&Tunnel_demo_s);
}
/* ${AOs::Tunnel::SM::active::demo} */
static QState Tunnel_demo(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::demo::TIME_TICK} */
        case TIME_TICK_SIG: {
            Tunnel_advance(me);
            if (me->blink_ctr != 0U) {
                /* add the text bitmap into the frame buffer */
                BSP_paintString((GAME_TUNNEL_WIDTH - 10U*6U)/2U,
                                (GAME_TUNNEL_HEIGHT - 4U)/2U,
                                "Press BTN0");
            }
            BSP_updateScreen();
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::demo::PLAYER_TRIGGER} */
        case PLAYER_TRIGGER_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { /* transition-action table */
                &Tunnel_playing_s, /* target state */
                {
                    Q_ACTION_CAST(&Tunnel_demo_x), /* exit */
                    Q_ACTION_CAST(&Tunnel_playing_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        /* ${AOs::Tunnel::SM::active::demo::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            --me->blink_ctr;
            /* ${AOs::Tunnel::SM::active::demo::Q_TIMEOUT::[me->blink_ctr==0]} */
            if (me->blink_ctr == 0) {
                static struct {
                    QMState const *target;
                    QActionHandler act[3];
                } const tatbl_ = { /* transition-action table */
                    &Tunnel_screen_saver_s, /* target state */
                    {
                        Q_ACTION_CAST(&Tunnel_demo_x), /* exit */
                        Q_ACTION_CAST(&Tunnel_screen_saver_i), /* init.tran. */
                        Q_ACTION_CAST(0) /* zero terminator */
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::active::playing} ......................................*/
/* ${AOs::Tunnel::SM::active::playing} */
static QState Tunnel_playing_e(Tunnel * const me) {
    me->wall_gap = GAME_WALLS_GAP_Y;
    QACTIVE_POST(&AO_Ship, TAKE_OFF_SIG, 0);
    return QM_ENTRY(&Tunnel_playing_s);
}
/* ${AOs::Tunnel::SM::active::playing} */
static QState Tunnel_playing_x(Tunnel * const me) {
    Tunnel_dispatchToAllMines(me, MINE_RECYCLE_SIG, 0);
    return QM_EXIT(&Tunnel_playing_s);
}
/* ${AOs::Tunnel::SM::active::playing} */
static QState Tunnel_playing(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::playing::TIME_TICK} */
        case TIME_TICK_SIG: {
            /* render this frame on the display */
            BSP_updateScreen();
            Tunnel_advance(me);
            Tunnel_plantMine(me);
            Tunnel_dispatchToAllMines(me, Q_SIG(me), Q_PAR(me));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::playing::SHIP_IMG} */
        case SHIP_IMG_SIG: {
            uint8_t x   = (uint8_t)Q_PAR(me);
            uint8_t y   = (uint8_t)(Q_PAR(me) >> 8);
            uint8_t bmp = (uint8_t)(Q_PAR(me) >> 16);

            /* did the Ship/Missile hit the tunnel wall? */
            if (BSP_isWallHit(bmp, x, y)) {
                QACTIVE_POST(&AO_Ship, HIT_WALL_SIG, 0);
            }
            BSP_paintBitmap(x, y, bmp);
            Tunnel_dispatchToAllMines(me, Q_SIG(me), Q_PAR(me));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::playing::MISSILE_IMG} */
        case MISSILE_IMG_SIG: {
            uint8_t x   = (uint8_t)Q_PAR(me);
            uint8_t y   = (uint8_t)(Q_PAR(me) >> 8);
            uint8_t bmp = (uint8_t)(Q_PAR(me) >> 16);

            /* did the Ship/Missile hit the tunnel wall? */
            if (BSP_isWallHit(bmp, x, y)) {
                QACTIVE_POST(&AO_Missile, HIT_WALL_SIG, 0);
            }
            BSP_paintBitmap(x, y, bmp);
            Tunnel_dispatchToAllMines(me, Q_SIG(me), Q_PAR(me));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::playing::MINE_IMG} */
        case MINE_IMG_SIG: {
            BSP_paintBitmap((uint8_t)Q_PAR(me),
                            (uint8_t)(Q_PAR(me) >> 8),
                            (uint8_t)(Q_PAR(me) >> 16));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::playing::EXPLOSION} */
        case EXPLOSION_SIG: {
            BSP_paintBitmap((uint8_t)Q_PAR(me),
                            (uint8_t)(Q_PAR(me) >> 8),
                            (uint8_t)(Q_PAR(me) >> 16));
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::playing::SCORE} */
        case SCORE_SIG: {
            BSP_updateScore((uint16_t)Q_PAR(me));
            /* increase difficulty of the game:
            * the tunnel gets narrower as the score goes up
            */
            me->wall_gap = (uint8_t)(GAME_WALLS_GAP_Y
                              - (uint16_t)Q_PAR(me)/100U);
            if (me->wall_gap < GAME_WALLS_MIN_GAP_Y) {
                me->wall_gap = GAME_WALLS_MIN_GAP_Y;
            }
            status_ = QM_HANDLED();
            break;
        }
        /* ${AOs::Tunnel::SM::active::playing::GAME_OVER} */
        case GAME_OVER_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { /* transition-action table */
                &Tunnel_game_over_s, /* target state */
                {
                    Q_ACTION_CAST(&Tunnel_playing_x), /* exit */
                    Q_ACTION_CAST(&Tunnel_game_over_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            BSP_clearWalls();
            BSP_updateScore((uint16_t)Q_PAR(me));
            BSP_updateScreen();
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::active::game_over} ....................................*/
/* ${AOs::Tunnel::SM::active::game_over} */
static QState Tunnel_game_over_e(Tunnel * const me) {
    QActive_armX(&me->super, 0U,
        BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U); /* periodic */
    me->blink_ctr = 5U*2U; /* 5s timeout */
    BSP_paintString((GAME_TUNNEL_WIDTH - 6U * 9U) / 2U,
                    (GAME_TUNNEL_HEIGHT / 2U) - 4U,
                    "Game Over");
    BSP_updateScreen();
    return QM_ENTRY(&Tunnel_game_over_s);
}
/* ${AOs::Tunnel::SM::active::game_over} */
static QState Tunnel_game_over_x(Tunnel * const me) {
    QActive_disarmX(&me->super, 0U);
    BSP_updateScore(0); /* update the score on the display */
    return QM_EXIT(&Tunnel_game_over_s);
}
/* ${AOs::Tunnel::SM::active::game_over} */
static QState Tunnel_game_over(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::game_over::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            --me->blink_ctr;
            BSP_paintString((GAME_TUNNEL_WIDTH - 6U*9U) / 2U,
                            (GAME_TUNNEL_HEIGHT / 2U) - 4U,
                            (((me->blink_ctr & 1) != 0)
                             ? "Game Over"
                             : "         "));
            BSP_updateScreen();
            /* ${AOs::Tunnel::SM::active::game_over::Q_TIMEOUT::[me->blink_ctr==0]} */
            if (me->blink_ctr == 0) {
                static struct {
                    QMState const *target;
                    QActionHandler act[3];
                } const tatbl_ = { /* transition-action table */
                    &Tunnel_demo_s, /* target state */
                    {
                        Q_ACTION_CAST(&Tunnel_game_over_x), /* exit */
                        Q_ACTION_CAST(&Tunnel_demo_e), /* entry */
                        Q_ACTION_CAST(0) /* zero terminator */
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::active::screen_saver} .................................*/
/* ${AOs::Tunnel::SM::active::screen_saver::initial} */
static QState Tunnel_screen_saver_i(Tunnel * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { /* transition-action table */
        &Tunnel_screen_saver_hide_s, /* target state */
        {
            Q_ACTION_CAST(&Tunnel_screen_saver_hide_e), /* entry */
            Q_ACTION_CAST(0) /* zero terminator */
        }
    };
    /* ${AOs::Tunnel::SM::active::screen_saver::initial} */
    return QM_TRAN_INIT(&tatbl_);
}
/* ${AOs::Tunnel::SM::active::screen_saver} */
static QState Tunnel_screen_saver(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::screen_saver::PLAYER_TRIGGER} */
        case PLAYER_TRIGGER_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { /* transition-action table */
                &Tunnel_demo_s, /* target state */
                {
                    Q_ACTION_CAST(&Tunnel_demo_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::active::screen_saver::screen_saver_hide} ..............*/
/* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_hide} */
static QState Tunnel_screen_saver_hide_e(Tunnel * const me) {
    BSP_displayOff();  /* power down the display */
    QActive_armX(&me->super, 0U, BSP_TICKS_PER_SEC*3U, 0U); /* 3 sec */
    return QM_ENTRY(&Tunnel_screen_saver_hide_s);
}
/* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_hide} */
static QState Tunnel_screen_saver_hide_x(Tunnel * const me) {
    QActive_disarmX(&me->super, 0U);
    BSP_displayOn(); /* power up the display */
    return QM_EXIT(&Tunnel_screen_saver_hide_s);
}
/* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_hide} */
static QState Tunnel_screen_saver_hide(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_hid~::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { /* transition-action table */
                &Tunnel_screen_saver_show_s, /* target state */
                {
                    Q_ACTION_CAST(&Tunnel_screen_saver_hide_x), /* exit */
                    Q_ACTION_CAST(&Tunnel_screen_saver_show_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::active::screen_saver::screen_saver_show} ..............*/
/* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_show} */
static QState Tunnel_screen_saver_show_e(Tunnel * const me) {
    uint32_t rnd = BSP_random();
    /* clear the screen frame buffer */
    BSP_clearFB();
    BSP_paintString((uint8_t)(rnd % (GAME_TUNNEL_WIDTH - 10U*6U)),
                    (uint8_t) (rnd % (GAME_TUNNEL_HEIGHT - 8U)),
                    "Press BTN0");
    BSP_updateScreen();
    QActive_armX(&me->super, 0U, BSP_TICKS_PER_SEC/3U, 0U);/* 1/3 sec */
    return QM_ENTRY(&Tunnel_screen_saver_show_s);
}
/* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_show} */
static QState Tunnel_screen_saver_show_x(Tunnel * const me) {
    QActive_disarmX(&me->super, 0U);
    BSP_clearFB();
    BSP_updateScreen();
    return QM_EXIT(&Tunnel_screen_saver_show_s);
}
/* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_show} */
static QState Tunnel_screen_saver_show(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::Tunnel::SM::active::screen_saver::screen_saver_sho~::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { /* transition-action table */
                &Tunnel_screen_saver_hide_s, /* target state */
                {
                    Q_ACTION_CAST(&Tunnel_screen_saver_show_x), /* exit */
                    Q_ACTION_CAST(&Tunnel_screen_saver_hide_e), /* entry */
                    Q_ACTION_CAST(0) /* zero terminator */
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
/*${AOs::Tunnel::SM::final} ................................................*/
/* ${AOs::Tunnel::SM::final} */
static QState Tunnel_final_e(Tunnel * const me) {
    /* clear the screen */
    BSP_clearFB();
    BSP_updateScreen();
    QF_stop(); /* stop QF and cleanup */
    (void)me; /* avoid compiler warning in case 'me' is not used */
    return QM_ENTRY(&Tunnel_final_s);
}
/* ${AOs::Tunnel::SM::final} */
static QState Tunnel_final(Tunnel * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    (void)me; /* avoid compiler warning in case 'me' is not used */
    return status_;
}
