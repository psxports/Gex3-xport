#include "psx.h"
#include "xport.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>

enum G3ReviewedLevelModule
{
    G3LevelModule_icecube = 0,
    G3LevelModule_fish = 1,
    G3LevelModule_gatgun = 2,
    G3LevelModule_kanga = 3,
    G3LevelModule_morface = 4,
    G3LevelModule_burro = 5,
    G3LevelModule_anigunt = 6,
    G3LevelModule_door = 7,
    G3LevelModule_bouncer = 8,
    G3LevelModule_schest = 9,
    G3LevelModule_cowboy = 10,
    G3LevelModule_barrel = 11,
    G3LevelModule_ebfire = 12,
    G3LevelModule_scannon = 13,
    G3LevelModule_cowboyb = 14,
    G3LevelModule_snow96w = 15,
    G3LevelModule_egen = 16,
    G3LevelModule_snwboard = 17,
    G3LevelModule_tumble = 18,
    G3LevelModule_rahstaf = 19,
    G3LevelModule_attach = 20,
    G3LevelModule_ctimer = 21,
    G3LevelModule_funplat = 22,
    G3LevelModule_sucky = 23,
    G3LevelModule_fxgen = 24,
    G3LevelModule_camel = 25,
    G3LevelModule_plank = 26,
    G3LevelModule_tank = 27,
    G3LevelModule_tracker = 28,
    G3LevelModule_guy = 29,
    G3LevelModule_chicken = 30,
    G3LevelModule_teeter = 31,
    G3LevelModule_spnplat = 32,
    G3LevelModule_hank = 33,
    G3LevelModule_shittrn = 34,
    G3LevelModule_clacker = 35,
    G3LevelModule_litball = 36,
    G3LevelModule_crmbplt = 37,
    G3LevelModule_bird = 38,
    G3LevelModule_mstrdbg = 39,
    G3LevelModule_zipline = 40,
    G3LevelModule_pirate = 41,
    G3LevelModule_rezntro = 42,
    G3LevelModule_pongball = 43,
    G3LevelModule_gxspit = 44,
    G3LevelModule_chunky = 45,
    G3LevelModule_wakball = 46,
    G3LevelModule_brkblok = 47,
    G3LevelModule_scrank = 48,
    G3LevelModule_shooter = 49,
    G3LevelModule_fallobj = 50,
    G3LevelModule_santa = 51,
    G3LevelModule_boinger = 52,
    G3LevelModule_looky = 53,
    G3LevelModule_covrobj = 54,
    G3LevelModule_allwhack = 55,
    G3LevelModule_waterfan = 56,
    G3LevelModule_alfred = 57,
    G3LevelModule_bomber = 58,
    G3LevelModule_cactus = 59,
    G3LevelModule_hitchup = 60,
    G3LevelModule_utaker = 61,
    G3LevelModule_airpod = 62,
    G3LevelModule_btimer = 63,
    G3LevelModule_fallgen = 64,
    G3LevelModule_monster = 65,
    G3LevelModule_g3logo = 66,
    G3LevelModule_chunk = 67,
    G3LevelModule_valtcode = 68,
    G3LevelModule_canball = 69,
    G3LevelModule_launcher = 70,
    G3LevelModule_proxsig = 71,
    G3LevelModule_coin = 72,
    G3LevelModule_gator = 73,
    G3LevelModule_leafgen = 74,
    G3LevelModule_anibomb = 75,
    G3LevelModule_gngcan = 76,
    G3LevelModule_fan = 77,
    G3LevelModule_lennyb = 78,
    G3LevelModule_canbalb = 79,
    G3LevelModule_missile = 80,
    G3LevelModule_tformr = 81
};

uint32 G3IceCubeEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3FishEvent(uint32 object, uint32 message, uint32 guest_stack);
uint32 G3GatgunEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3KangaEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3MorphFaceEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3BurroEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3AniguntEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3DoorEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3BouncerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3SchestEvent(uint32 object, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3CowboyEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3BarrelEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3EbfireEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3ScannonEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3CowboyBEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3SnowWeaponEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3EgenEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3SnowboardEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3TumbleEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3RahstafEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3AttachEvent(uint32 object, uint32 message, uint32 guest_stack);
uint32 G3CountdownEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3FunPlatformEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3SuckyEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3EffectGeneratorEvent(uint32 object, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3CamelEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3PlankEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3TankEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3TrackerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3GuyEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3ChickenEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3TeeterEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3SpnplatEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3HankEvent0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3ShittrnEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack);
uint32 G3ClackerEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3LitballEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3CrumblingPlatformEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3BirdEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgEntry0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3ZiplineEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3PirateEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3IntroEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack);
uint32 G3PongballEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3GxSpitEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3ChunkyEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3WakballEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3BrkblokEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3ScrankEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3ShooterEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3FallingObjectEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3SantaEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3BoingerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3LookyEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3CoverEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack);
uint32 G3AllwhackEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3WaterFanEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3AlfredEvent(uint32 object, uint32 message, uint32 guest_stack);
uint32 G3BomberEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3CactusEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3HitchupEvent(uint32 object, uint32 message, uint32 guest_stack);
uint32 G3UtakerEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3AirpodEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3BombTimerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3FallgenEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3MonsterEntry0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3LogoEvent(uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3ChunkEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3VaultCodeEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3CannonBallEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3LauncherEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3ProxsigEvent(uint32 object, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3CoinEvent(uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3GatorEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3LeafGeneratorEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3AnimatedBombEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3GangCannonEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3FanEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3LennyBEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3CanbalbEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3MissileEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3TformrEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);

uint32 G3InvokeReviewedLevelEntry(uint32 identity_id, uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    switch (identity_id)
    {
    case G3LevelModule_icecube:
        return G3IceCubeEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_fish:
        return G3FishEvent(object, message, guest_stack);
    case G3LevelModule_gatgun:
        return G3GatgunEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_kanga:
        return G3KangaEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_morface:
        return G3MorphFaceEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_burro:
        return G3BurroEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_anigunt:
        return G3AniguntEvent(object, message, module_base, guest_stack);
    case G3LevelModule_door:
        return G3DoorEvent(object, game, message, guest_stack);
    case G3LevelModule_bouncer:
        return G3BouncerEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_schest:
        return G3SchestEvent(object, message, payload, guest_stack);
    case G3LevelModule_cowboy:
        return G3CowboyEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_barrel:
        return G3BarrelEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_ebfire:
        return G3EbfireEvent(object, message, module_base, guest_stack);
    case G3LevelModule_scannon:
        return G3ScannonEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_cowboyb:
        return G3CowboyBEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_snow96w:
        return G3SnowWeaponEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_egen:
        return G3EgenEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_snwboard:
        return G3SnowboardEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_tumble:
        return G3TumbleEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_rahstaf:
        return G3RahstafEvent(object, game, message, guest_stack);
    case G3LevelModule_attach:
        return G3AttachEvent(object, message, guest_stack);
    case G3LevelModule_ctimer:
        return G3CountdownEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_funplat:
        return G3FunPlatformEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_sucky:
        return G3SuckyEvent(object, game, message, guest_stack);
    case G3LevelModule_fxgen:
        return G3EffectGeneratorEvent(object, message, payload, module_base, guest_stack);
    case G3LevelModule_camel:
        return G3CamelEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_plank:
        return G3PlankEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_tank:
        return G3TankEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_tracker:
        return G3TrackerEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_guy:
        return G3GuyEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_chicken:
        return G3ChickenEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_teeter:
        return G3TeeterEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_spnplat:
        return G3SpnplatEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_hank:
        return G3HankEvent0000(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_shittrn:
        return G3ShittrnEvent(module_base, object, message, guest_stack);
    case G3LevelModule_clacker:
        return G3ClackerEvent(object, game, message, guest_stack);
    case G3LevelModule_litball:
        return G3LitballEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_crmbplt:
        return G3CrumblingPlatformEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_bird:
        return G3BirdEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_mstrdbg:
        return G3MstrdbgEntry0000(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_zipline:
        return G3ZiplineEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_pirate:
        return G3PirateEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_rezntro:
        return G3IntroEvent(module_base, object, message, guest_stack);
    case G3LevelModule_pongball:
        return G3PongballEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_gxspit:
        return G3GxSpitEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_chunky:
        return G3ChunkyEvent(object, message, module_base, guest_stack);
    case G3LevelModule_wakball:
        return G3WakballEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_brkblok:
        return G3BrkblokEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_scrank:
        return G3ScrankEvent(object, game, message, guest_stack);
    case G3LevelModule_shooter:
        return G3ShooterEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_fallobj:
        return G3FallingObjectEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_santa:
        return G3SantaEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_boinger:
        return G3BoingerEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_looky:
        return G3LookyEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_covrobj:
        return G3CoverEvent(module_base, object, message, guest_stack);
    case G3LevelModule_allwhack:
        return G3AllwhackEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_waterfan:
        return G3WaterFanEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_alfred:
        return G3AlfredEvent(object, message, guest_stack);
    case G3LevelModule_bomber:
        return G3BomberEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_cactus:
        return G3CactusEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_hitchup:
        return G3HitchupEvent(object, message, guest_stack);
    case G3LevelModule_utaker:
        return G3UtakerEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_airpod:
        return G3AirpodEvent(object, message, module_base, guest_stack);
    case G3LevelModule_btimer:
        return G3BombTimerEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_fallgen:
        return G3FallgenEvent(object, game, message, guest_stack);
    case G3LevelModule_monster:
        return G3MonsterEntry0000(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_g3logo:
        return G3LogoEvent(module_base, object, game, message, guest_stack);
    case G3LevelModule_chunk:
        return G3ChunkEvent(object, message, module_base, guest_stack);
    case G3LevelModule_valtcode:
        return G3VaultCodeEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_canball:
        return G3CannonBallEvent(object, game, message, payload, guest_stack);
    case G3LevelModule_launcher:
        return G3LauncherEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_proxsig:
        return G3ProxsigEvent(object, message, payload, module_base, guest_stack);
    case G3LevelModule_coin:
        return G3CoinEvent(module_base, object, game, message, guest_stack);
    case G3LevelModule_gator:
        return G3GatorEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_leafgen:
        return G3LeafGeneratorEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_anibomb:
        return G3AnimatedBombEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_gngcan:
        return G3GangCannonEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_fan:
        return G3FanEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_lennyb:
        return G3LennyBEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_canbalb:
        return G3CanbalbEvent(object, game, message, module_base, guest_stack);
    case G3LevelModule_missile:
        return G3MissileEvent(object, game, message, payload, module_base, guest_stack);
    case G3LevelModule_tformr:
        return G3TformrEvent(object, game, message, module_base, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved level module identity %u\n", (unsigned int)identity_id);
#endif
        }
        abort();
    }
}


#include "xport.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>

enum G3ReviewedCallbackModule
{
    G3CallbackModule_fxgen = 24,
    G3CallbackModule_chunky = 45,
    G3CallbackModule_brkblok = 47,
    G3CallbackModule_looky = 53,
    G3CallbackModule_covrobj = 54,
    G3CallbackModule_g3logo = 66,
    G3CallbackModule_leafgen = 74,
    G3CallbackModule_canbalb = 79
};

uint32 G3BrkblokFragmentTick(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3EffectGeneratorOscillate(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3CanbalbCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3ChunkyRender(uint32 camera, uint32 object, uint32 packets, uint32 guest_stack);
uint32 G3LookyRender(uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack);
uint32 G3CoverRender(uint32 scene, uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack);
uint32 G3LogoRender(uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack);
uint32 G3LeafEffectUpdate(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3LeafEffectInitialize(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack);
bool G3LevelCodeResolve(uint32 target, uint32 *identity_id, uint32 *module_base, uint32 *block_offset);
uint32 G3InvokeReviewedLevelEntry(uint32 identity_id, uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);

void G3DispatchObjectRender(uint32 target, uint32 scene, uint32 object, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    // The callee retains its original ordering-table load from guest_stack + 0x10
    switch (target)
    {
    case 0x8006c110:
        (void)G3Function8006C110(scene, object, vertices, packets, guest_stack);
        return;
    default:
        {
            uint32 identity_id;
            uint32 module_base;
            uint32 offset;
            if (G3LevelCodeResolve(target, &identity_id, &module_base, &offset))
            {
                if (identity_id == G3CallbackModule_chunky && offset == 0x86c)
                {
                    (void)G3ChunkyRender(scene, object, packets, guest_stack);
                    return;
                }
                if (identity_id == G3CallbackModule_looky && offset == 0x36c)
                {
                    (void)G3LookyRender(object, vertices, packets, guest_stack);
                    return;
                }
                if (identity_id == G3CallbackModule_covrobj && offset == 0x31c)
                {
                    (void)G3CoverRender(scene, object, vertices, packets, guest_stack);
                    return;
                }
                if (identity_id == G3CallbackModule_g3logo && offset == 0x2b0)
                {
                    (void)G3LogoRender(object, vertices, packets, guest_stack);
                    return;
                }
            }
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved object render target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}

uint32 G3DispatchPoolCallback(uint32 target, uint32 effect, uint32 owner, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8001785c:
        return G3Function8001785C(effect, owner, guest_stack);
    case 0x80025644:
        return G3Function80025644(effect, owner, guest_stack);
    case 0x80039158:
        return G3Function80039158(effect, owner, guest_stack);
    case 0x8004197c:
        return G3Function8004197C(effect, owner, guest_stack);
    case 0x80041de0:
        return G3Function80041DE0(effect, owner, guest_stack);
    case 0x800436f0:
        return G3Function800436F0(effect, owner, guest_stack);
    case 0x80043f7c:
        return G3Function80043F7C(effect, owner, guest_stack);
    case 0x80044f14:
        return G3Function80044F14(effect, owner, guest_stack);
    case 0x80044f7c:
        return G3Function80044F7C(effect, owner, guest_stack);
    case 0x8004502c:
        return G3Function8004502C(effect, owner, guest_stack);
    case 0x8004508c:
        return G3Function8004508C(effect, owner, guest_stack);
    case 0x8004512c:
        return G3Function8004512C(effect, owner, guest_stack);
    case 0x800456ec:
        return G3Function800456EC(effect, owner, guest_stack);
    case 0x800459d8:
        return G3Function800459D8(effect, owner, guest_stack);
    default:
        {
            uint32 identity_id;
            uint32 module_base;
            uint32 offset;
            if (G3LevelCodeResolve(target, &identity_id, &module_base, &offset))
            {
                if (identity_id == G3CallbackModule_fxgen && offset == 0x128)
                    return G3EffectGeneratorOscillate(effect, owner, guest_stack);
                if (identity_id == G3CallbackModule_brkblok && offset == 0x85c)
                    return G3BrkblokFragmentTick(effect, owner, guest_stack);
                if (identity_id == G3CallbackModule_leafgen && offset == 0x378)
                    return G3LeafEffectUpdate(effect, owner, guest_stack);
            }
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved pool callback target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}

uint32 G3DispatchEffectInitializer(uint32 target, uint32 effect, uint32 callback, uint32 storage, uint32 argument3, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80017680:
        return G3Function80017680(effect, callback, storage, guest_stack);
    case 0x80044bc4:
        return G3Function80044BC4(effect, callback, storage, argument3, guest_stack);
    default:
        {
            uint32 identity_id;
            uint32 module_base;
            uint32 offset;
            if (G3LevelCodeResolve(target, &identity_id, &module_base, &offset) && identity_id == G3CallbackModule_leafgen && offset == 0x11c)
                return G3LeafEffectInitialize(effect, callback, storage, guest_stack);
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved effect initializer target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}

uint32 G3DispatchStateEvent(uint32 target, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8003774c:
        return G3Function8003774C(object, game, message, guest_stack);
    case 0x8003acfc:
        return G3Function8003ACFC(object, game, message, payload, guest_stack);
    case 0x80045cac:
        return G3Function80045CAC(object, game, message, payload, guest_stack);
    case 0x8005a2c8:
        return G3Function8005A2C8(object, game, message, payload, guest_stack);
    case 0x80063bb4:
        return G3Function80063BB4(object, game, message, payload, guest_stack);
    case 0x800647a4:
        return G3Function800647A4(object, game, message, payload, guest_stack);
    case 0x800659b0:
        return G3Function800659B0(object, game, message, payload, guest_stack);
    case 0x80065d54:
        return G3Function80065D54(object, game, message, guest_stack);
    case 0x8006b8cc:
        return G3Function8006B8CC(object, game, message, payload, guest_stack);
    case 0x80076450:
        return G3Function80076450(object, game, message, payload, guest_stack);
    case 0x8007801c:
        return G3Function8007801C(object, game, message, payload, guest_stack);
    case 0x8007c5d8:
        return G3Function8007C5D8(object, game, message, payload, guest_stack);
    case 0x80080610:
        return G3Function80080610(object, game, message, guest_stack);
    case 0x8008095c:
        return G3Function8008095C(object, game, message, payload, guest_stack);
    case 0x80081c84:
        return G3Function80081C84(object, game, message, guest_stack);
    case 0x800836b0:
        return G3Function800836B0(object, game, message, guest_stack);
    case 0x80084e1c:
        return G3Function80084E1C(object, game, message, payload, guest_stack);
    case 0x80060074:
        return G3Function80060074(object, game, message, payload, guest_stack);
    case 0x80058298:
        return G3Function80058298(object, game, message, payload, guest_stack);
    case 0x8004e5f8:
        return G3Function8004E5F8(object, game, message, payload, guest_stack);
    case 0x8004e994:
        return G3Function8004E994(object, message, payload, guest_stack);
    case 0x8004f5e4:
        return G3Function8004F5E4(object, game, message, payload, guest_stack);
    case 0x80050024:
        return G3Function80050024(object, game, message, payload, guest_stack);
    case 0x8005153c:
        return G3Function8005153C(object, game, message, payload, guest_stack);
    case 0x8005180c:
        return G3Function8005180C(object, message, guest_stack);
    case 0x8005235c:
        return G3Function8005235C(object, game, message, payload, guest_stack);
    case 0x8005402c:
        return G3Function8005402C();
    case 0x800551d0:
        return G3Function800551D0(object, message, payload, guest_stack);
    case 0x80055f18:
        return G3Function80055F18(object, message, payload, guest_stack);
    default:
        {
            uint32 identity_id;
            uint32 module_base;
            uint32 block_offset;
            if (G3LevelCodeResolve(target, &identity_id, &module_base, &block_offset))
            {
                if (block_offset == 0)
                    return G3InvokeReviewedLevelEntry(identity_id, module_base, object, game, message, payload, guest_stack);
                if (identity_id == G3CallbackModule_canbalb && block_offset == 0x27c)
                    return G3CanbalbCollide(object, game, module_base, guest_stack);
            }
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved state event target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}


uint32 G3DispatchAudioEvent(uint32 target, uint32 event, uint32 bank, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80049df4:
        G3Function80049DF4();
        return target;
    case 0x80049dfc:
        return G3Function80049DFC(event, bank);
    case 0x8004aa4c:
        G3Function8004AA4C();
        return target;
    case 0x8004aa54:
        return G3Function8004AA54(event, bank, guest_stack);
    case 0x8004aa94:
        return G3Function8004AA94(event, bank);
    case 0x8004aaac:
        G3Function8004AAAC();
        return target;
    case 0x8004aab4:
        return G3Function8004AAB4(event, bank);
    case 0x8004aafc:
        G3Function8004AAFC();
        return target;
    case 0x8004ab04:
        return G3Function8004AB04(event, bank);
    case 0x8004ab48:
        return G3Function8004AB48(event, bank);
    case 0x8004ab98:
        return G3Function8004AB98(event, bank);
    case 0x8004abe8:
        return G3Function8004ABE8(event, bank, guest_stack);
    case 0x8004ac30:
        G3Function8004AC30();
        return target;
    case 0x8004ad00:
        return G3Function8004AD00(event, bank);
    case 0x8004ad20:
        return G3Function8004AD20(event, bank);
    case 0x8004ad74:
        return G3Function8004AD74(event, bank, guest_stack);
    case 0x8004adc4:
        G3Function8004ADC4();
        return target;
    case 0x8004adcc:
        G3Function8004ADCC();
        return target;
    case 0x8004add4:
        return G3Function8004ADD4(event, bank, guest_stack);
    case 0x8004ae40:
        G3Function8004AE40();
        return target;
    case 0x8004ae48:
        return G3Function8004AE48(event, bank, guest_stack);
    case 0x8004aea0:
        return G3Function8004AEA0(event, bank);
    case 0x8004aec4:
        return G3Function8004AEC4(event, bank);
    case 0x8004aee8:
        return G3Function8004AEE8(event, bank, guest_stack);
    case 0x8004af0c:
        return G3Function8004AF0C(event, bank);
    case 0x8004af30:
        G3Function8004AF30();
        return target;
    case 0x8004af38:
        G3Function8004AF38();
        return target;
    case 0x8004af40:
        return G3Function8004AF40(event, bank);
    case 0x8004af74:
        return G3Function8004AF74(event, bank);
    case 0x8004afa8:
        return G3Function8004AFA8(event, bank);
    case 0x8004afe4:
        return G3Function8004AFE4(event, bank);
    case 0x8004b020:
        return G3Function8004B020(event, bank);
    case 0x8004b040:
        return G3Function8004B040(event, bank);
    case 0x8004b064:
        return G3Function8004B064(event, bank);
    case 0x8004b094:
        return G3Function8004B094(event, bank);
    case 0x8004b0c4:
        return G3Function8004B0C4(event, bank);
    case 0x8004b0f4:
        return G3Function8004B0F4(event, bank);
    case 0x8004b174:
        return G3Function8004B174(event, bank);
    case 0x8004b1a4:
        G3Function8004B1A4();
        return target;
    case 0x8004b1ac:
        G3Function8004B1AC();
        return target;
    case 0x8004b1b4:
        G3Function8004B1B4();
        return target;
    case 0x8004b1bc:
        G3Function8004B1BC();
        return target;
    case 0x8004b1c4:
        return G3Function8004B1C4(event, bank);
    case 0x8004b1dc:
        G3Function8004B1DC();
        return target;
    case 0x8004b1e4:
        G3Function8004B1E4();
        return target;
    case 0x8004b1ec:
        G3Function8004B1EC();
        return target;
    case 0x8004b1f4:
        G3Function8004B1F4();
        return target;
    case 0x8004b1fc:
        G3Function8004B1FC();
        return target;
    case 0x8004b204:
        G3Function8004B204();
        return target;
    case 0x8004b20c:
        G3Function8004B20C();
        return target;
    case 0x8004b214:
        return G3Function8004B214(event, bank);
    case 0x8004b244:
        return G3Function8004B244(event, bank);
    case 0x8004b2b4:
        return G3Function8004B2B4(event, bank);
    case 0x8004b2e4:
        return G3Function8004B2E4(event, bank);
    case 0x8004b338:
        return G3Function8004B338(event, bank);
    case 0x8004b36c:
        return G3Function8004B36C(event, bank);
    case 0x8004b3a0:
        G3Function8004B3A0();
        return target;
    case 0x8004b3a8:
        return G3Function8004B3A8(event, bank);
    case 0x8004b3dc:
        G3Function8004B3DC();
        return target;
    case 0x8004b3e4:
        G3Function8004B3E4();
        return target;
    case 0x8004b3ec:
        return G3Function8004B3EC(event, bank);
    case 0x8004b41c:
        return G3Function8004B41C(event, bank);
    case 0x8004b44c:
        return G3Function8004B44C(event);
    case 0x8004b478:
        return G3Function8004B478(event);
    case 0x8004b4b8:
        return G3Function8004B4B8(event);
    case 0x8004b4f0:
        return G3Function8004B4F0(event);
    case 0x8004b528:
        return G3Function8004B528(event);
    case 0x8004b560:
        return G3Function8004B560(event);
    case 0x8004b658:
        G3Function8004B658();
        return target;
    case 0x8004b660:
        G3Function8004B660();
        return target;
    case 0x8004b668:
        G3Function8004B668();
        return target;
    case 0x8004b670:
        G3Function8004B670();
        return target;
    case 0x8004b678:
        return G3Function8004B678(event, bank);
    case 0x8004b6a0:
        return G3Function8004B6A0(event, bank);
    case 0x8004b6c8:
        return G3Function8004B6C8(event, bank);
    case 0x8004b710:
        return G3Function8004B710(event, bank);
    case 0x8004b758:
        return G3Function8004B758(event, bank);
    case 0x8004b7a4:
        return G3Function8004B7A4(event, bank);
    case 0x8004b7f0:
        return G3Function8004B7F0(event, bank);
    case 0x8004b83c:
        return G3Function8004B83C(event, bank);
    case 0x8004b888:
        return G3Function8004B888(event, bank);
    case 0x8004b8d4:
        return G3Function8004B8D4(event, bank);
    case 0x8004b924:
        return G3Function8004B924(event, bank);
    case 0x8004b938:
        return G3Function8004B938(event, bank);
    case 0x8004b94c:
        return G3Function8004B94C(event, bank);
    case 0x8004b960:
        return G3Function8004B960(event, bank, guest_stack);
    case 0x8004b9c0:
        return G3Function8004B9C0(event, guest_stack);
    case 0x8004ba78:
        return G3Function8004BA78(event, guest_stack);
    case 0x8004bae8:
        return G3Function8004BAE8(event, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved audio event target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchObjectEffect(uint32 target, uint32 object, uint32 effect, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8003d4e4:
        return G3Function8003D4E4(object, effect, guest_stack);
    case 0x8003d5e8:
        return G3Function8003D5E8(object, effect, guest_stack);
    case 0x8003d6f0:
        return G3Function8003D6F0(object, effect, guest_stack);
    case 0x8003d7f4:
        return G3Function8003D7F4(object, effect, guest_stack);
    case 0x8003d8fc:
        return G3Function8003D8FC(object, effect, guest_stack);
    case 0x8003da70:
        return G3Function8003DA70(object, effect, guest_stack);
    case 0x8003dc08:
        return G3Function8003DC08(object, effect, guest_stack);
    case 0x8003de64:
        return G3Function8003DE64(object, effect, guest_stack);
    case 0x8003e0ac:
        return G3Function8003E0AC(object, effect, guest_stack);
    case 0x8003e30c:
        G3Function8003E30C();
        return target;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved object effect target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchBoxPlaneCheck(uint32 target, uint32 point, uint32 box, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8005f7d4:
        return G3Function8005F7D4(point, box);
    case 0x8005f830:
        return G3Function8005F830(point, box);
    case 0x8005f88c:
        return G3Function8005F88C(point, box);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved box plane target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchTimedObjectCallback(uint32 target, uint32 object, uint32 argument1, uint32 argument2, uint32 effect, uint32 guest_stack)
{
    return G3DispatchStateEvent(target, object, argument1, argument2, effect, guest_stack);
}

uint32 G3DispatchMenuPageCallback(uint32 target, uint32 context, uint32 selected, uint32 second_glyph, uint32 third_glyph, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80068d90:
        return G3Function80068D90(context, selected, third_glyph, guest_stack);
    case 0x80068c54:
        return G3Function80068C54(context, selected, third_glyph, guest_stack);
    case 0x80069000:
        return G3Function80069000(context, selected, third_glyph, guest_stack);
    case 0x8006954c:
        return G3Function8006954C(context, third_glyph, guest_stack);
    case 0x80069dd4:
        return G3Function80069DD4(context, selected, guest_stack);
    case 0x80069e8c:
        return G3Function80069E8C(context, selected, guest_stack);
    case 0x80069f54:
        return G3Function80069F54(context, selected, guest_stack);
    case 0x8006a088:
        return G3Function8006A088(context, selected, guest_stack);
    case 0x8006a244:
        return G3Function8006A244(context, selected, guest_stack);
    case 0x8006a350:
        return G3Function8006A350(context, selected, guest_stack);
    case 0x8006a3f8:
        return G3Function8006A3F8(context, selected, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved menu page target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchMenuRowCallback(uint32 target, uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80068d38:
        return G3Function80068D38(context, selected, event, guest_stack);
    case 0x80068e2c:
        return G3Function80068E2C(context, selected, event, guest_stack);
    case 0x80068ec0:
        return G3Function80068EC0(context, selected, event, guest_stack);
    case 0x80068f1c:
        return G3Function80068F1C(context, selected, event, guest_stack);
    case 0x80068c1c:
        return G3Function80068C1C(context, selected, event, guest_stack);
    case 0x800694b4:
        return G3Function800694B4(context, selected, event, guest_stack);
    case 0x80069764:
        return G3Function80069764(context, selected, event, guest_stack);
    case 0x800699ac:
        return G3Function800699AC(context, selected, event, guest_stack);
    case 0x800699e4:
        return G3Function800699E4(context, selected, event, guest_stack);
    case 0x80069d40:
        return G3Function80069D40(context, selected, event, guest_stack);
    case 0x80069e3c:
        return G3Function80069E3C(context, selected, event, guest_stack);
    case 0x80069f0c:
        return G3Function80069F0C(context, selected, event, guest_stack);
    case 0x80069fd4:
        return G3Function80069FD4(context, selected, event, guest_stack);
    case 0x8006a1fc:
        return G3Function8006A1FC(context, selected, event, guest_stack);
    case 0x8006a308:
        return G3Function8006A308(context, selected, event, guest_stack);
    case 0x8006ade8:
        return G3Function8006ADE8(context, selected, event, guest_stack);
    case 0x8006ae60:
        return G3Function8006AE60(context, selected, event, guest_stack);
    case 0x8006af68:
        return G3Function8006AF68(context, selected, event, guest_stack);
    case 0x8006b08c:
        return G3Function8006B08C(context, selected, event, guest_stack);
    case 0x8006b0cc:
        return G3Function8006B0CC(context, selected, event, guest_stack);
    case 0x8006b170:
        return G3Function8006B170(context, selected, event, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved menu row target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3DispatchMenuExitCallback(uint32 target, uint32 context, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80068e98:
        (void)G3Function80068E98(context, guest_stack);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved menu exit target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchPathScore(uint32 target, uint32 position, uint32 state, uint32 context, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8004f85c:
        return G3Function8004F85C(position, context, guest_stack);
    case 0x8004faac:
        return G3Function8004FAAC(position, context, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved path score target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3DispatchPlayerUpdate(uint32 target, uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8006d8c4:
        (void)G3Function8006D8C4(object, game, data, controls, guest_stack);
        return;
    case 0x80072b0c:
        (void)G3Function80072B0C(object, game, data, controls, guest_stack);
        return;
    case 0x80074600:
        (void)G3Function80074600(object, game, data, controls, guest_stack);
        return;
    case 0x80074888:
        (void)G3Function80074888(object, game, data, guest_stack);
        return;
    case 0x800760f4:
        (void)G3Function800760F4(object, game, data, controls, guest_stack);
        return;
    case 0x800789c0:
        (void)G3Function800789C0(object, game, data, guest_stack);
        return;
    case 0x80078c04:
        (void)G3Function80078C04(object, game, data, controls, guest_stack);
        return;
    case 0x80078de8:
        (void)G3Function80078DE8(object, game, data);
        return;
    case 0x80078e40:
        (void)G3Function80078E40(object, game, data, controls, guest_stack);
        return;
    case 0x80079178:
        (void)G3Function80079178(object, game, data, controls, guest_stack);
        return;
    case 0x800792bc:
        (void)G3Function800792BC(object, game, data, controls, guest_stack);
        return;
    case 0x80079430:
        (void)G3Function80079430(object, game, data, controls, guest_stack);
        return;
    case 0x80079594:
        (void)G3Function80079594(object, game, data, controls, guest_stack);
        return;
    case 0x800796e0:
        (void)G3Function800796E0(object, game, data, controls, guest_stack);
        return;
    case 0x80079a90:
        (void)G3Function80079A90(object, game, data, controls, guest_stack);
        return;
    case 0x80079bc0:
        (void)G3Function80079BC0(object, game, data, controls, guest_stack);
        return;
    case 0x80079e70:
        (void)G3Function80079E70(object, game, data, controls, guest_stack);
        return;
    case 0x80079f40:
        (void)G3Function80079F40(object, game, data, controls, guest_stack);
        return;
    case 0x8007a19c:
        (void)G3Function8007A19C(object, game, data, controls, guest_stack);
        return;
    case 0x8007a2fc:
        (void)G3Function8007A2FC(object, game, data, controls, guest_stack);
        return;
    case 0x8007a5e8:
        (void)G3Function8007A5E8(object, game, data, controls, guest_stack);
        return;
    case 0x8007a708:
        (void)G3Function8007A708(object, game, data);
        return;
    case 0x8007a738:
        (void)G3Function8007A738(object, game, data, controls, guest_stack);
        return;
    case 0x8007a7f0:
        (void)G3Function8007A7F0(object, game, data, controls, guest_stack);
        return;
    case 0x8007a894:
        (void)G3Function8007A894(object, game, data, controls, guest_stack);
        return;
    case 0x8007a9a8:
        (void)G3Function8007A9A8(object, game, data, guest_stack);
        return;
    case 0x8007ac3c:
        (void)G3Function8007AC3C(object, game, data, controls, guest_stack);
        return;
    case 0x8007acf0:
        (void)G3Function8007ACF0(object, game, data, controls, guest_stack);
        return;
    case 0x8007ae0c:
        (void)G3Function8007AE0C(object, game, data, controls, guest_stack);
        return;
    case 0x8007ae78:
        (void)G3Function8007AE78(object, game, data, controls, guest_stack);
        return;
    case 0x8007af30:
        (void)G3Function8007AF30(object, game, data);
        return;
    case 0x8007afbc:
        (void)G3Function8007AFBC(object, game, data);
        return;
    case 0x8007b010:
        (void)G3Function8007B010(object, game, data, guest_stack);
        return;
    case 0x8007bab8:
        (void)G3Function8007BAB8(object, game, data, guest_stack);
        return;
    case 0x8007be98:
        (void)G3Function8007BE98(object, game, data, controls, guest_stack);
        return;
    case 0x8007eff8:
        (void)G3Function8007EFF8(object, game, data, guest_stack);
        return;
    case 0x8008018c:
        (void)G3Function8008018C(object, game, data, guest_stack);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved player update target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3DispatchDebugMenuAction(uint32 target, uint32 selected_entry, uint32 first_glyph, uint32 second_glyph, uint32 third_glyph, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80012720:
        (void)G3Function80012720(guest_stack);
        return;
    case 0x80012954:
        (void)G3Function80012954(guest_stack);
        return;
    case 0x800129cc:
        (void)G3Function800129CC(guest_stack);
        return;
    case 0x8003ef8c:
        (void)G3Function8003EF8C(guest_stack);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved debug menu target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchTerrainRender(uint32 target, uint32 context, uint32 level, uint32 camera, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    // The original caller already stores the ordering table at guest_stack + 0x10
    switch (target)
    {
    case 0x80084a64:
        return G3Function80084A64(context, level, camera, packets, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved terrain render target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchTimedScriptPoll(uint32 target, uint32 object, uint32 payload, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80016f50:
        return G3Function80016F50(object, payload, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved timed script poll target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3DispatchScriptValue(uint32 target, uint32 key, uint32 value, uint32 command, uint32 selector, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8003eb28:
        G3Function8003EB28();
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved script value target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3DispatchScriptCommand(uint32 target, uint32 object, uint32 command, uint32 guest_stack)
{
    switch (target)
    {
    case 0x800152a4:
        return G3Function800152A4(object, command, guest_stack);
    case 0x800152d4:
        return G3Function800152D4(object, command, guest_stack);
    case 0x800153c8:
        return G3Function800153C8(object, command, guest_stack);
    case 0x800153fc:
        return G3Function800153FC(object, command, guest_stack);
    case 0x8001543c:
        return G3Function8001543C(object, command);
    case 0x800154a4:
        return G3Function800154A4(object, command);
    case 0x800154d4:
        return G3Function800154D4();
    case 0x800154dc:
        return G3Function800154DC();
    case 0x800154e4:
        return G3Function800154E4(object, command);
    case 0x80015510:
        return G3Function80015510(object, command);
    case 0x8001553c:
        return G3Function8001553C(object, command, guest_stack);
    case 0x800155d4:
        return G3Function800155D4();
    case 0x800155dc:
        return G3Function800155DC(object, command);
    case 0x80015600:
        return G3Function80015600(object, command);
    case 0x80015614:
        return G3Function80015614(object, command, guest_stack);
    case 0x80015644:
        return G3Function80015644(object, command);
    case 0x80015668:
        return G3Function80015668(object, command);
    case 0x800157dc:
        return G3Function800157DC(object, command, guest_stack);
    case 0x8001580c:
        return G3Function8001580C(object, command);
    case 0x8001583c:
        return G3Function8001583C(object, command);
    case 0x8001586c:
        return G3Function8001586C(object, command);
    case 0x8001589c:
        return G3Function8001589C();
    case 0x800158a4:
        return G3Function800158A4();
    case 0x800158ac:
        return G3Function800158AC(object, command, guest_stack);
    case 0x800158e4:
        return G3Function800158E4(object, command, guest_stack);
    case 0x80015ba8:
        return G3Function80015BA8(object, command, guest_stack);
    case 0x80015ccc:
        return G3Function80015CCC(object, command);
    case 0x80015cfc:
        return G3Function80015CFC();
    case 0x80015d04:
        return G3Function80015D04(object, command);
    case 0x80015d40:
        return G3Function80015D40(object, command);
    case 0x80015df0:
        return G3Function80015DF0(object, command, guest_stack);
    case 0x80015e30:
        return G3Function80015E30(object, command, guest_stack);
    case 0x80015e64:
        return G3Function80015E64(object, command, guest_stack);
    case 0x80015e98:
        return G3Function80015E98(object, command, guest_stack);
    case 0x80015ecc:
        return G3Function80015ECC(object, command, guest_stack);
    case 0x80015f0c:
        return G3Function80015F0C(object, command, guest_stack);
    case 0x80015f4c:
        return G3Function80015F4C(object, command);
    case 0x80015f70:
        return G3Function80015F70(object, command, guest_stack);
    case 0x8001609c:
        return G3Function8001609C(object, command, guest_stack);
    case 0x80016174:
        return G3Function80016174(guest_stack);
    case 0x80016248:
        return G3Function80016248();
    case 0x80016310:
        return G3Function80016310(object, command);
    case 0x80016330:
        return G3Function80016330(object, command, guest_stack);
    case 0x80016574:
        return G3Function80016574(object, command, guest_stack);
    case 0x800165b8:
        return G3Function800165B8(object, command, guest_stack);
    case 0x80016820:
        return G3Function80016820();
    case 0x80016828:
        return G3Function80016828();
    case 0x80016888:
        return G3Function80016888();
    case 0x800168e8:
        return G3Function800168E8(object, command);
    case 0x80016908:
        return G3Function80016908(object, command);
    case 0x80016928:
        return G3Function80016928(object);
    case 0x80016954:
        return G3Function80016954(object);
    case 0x80016970:
        return G3Function80016970(object, command);
    case 0x8001698c:
        return G3Function8001698C(object, command);
    case 0x800169c8:
        return G3Function800169C8(object, command);
    case 0x80016a04:
        return G3Function80016A04(object, command);
    case 0x80016a24:
        return G3Function80016A24(object, command);
    case 0x80016a44:
        return G3Function80016A44(object, command);
    case 0x80016a68:
        return G3Function80016A68(object, command);
    case 0x80016aa0:
        return G3Function80016AA0(object, command);
    case 0x80016ae4:
        return G3Function80016AE4(object, command);
    case 0x80016b28:
        return G3Function80016B28(object, command);
    case 0x80016b6c:
        return G3Function80016B6C(object, command, guest_stack);
    case 0x80016bdc:
        return G3Function80016BDC(object, command, guest_stack);
    case 0x80016c4c:
        return G3Function80016C4C(object, command, guest_stack);
    case 0x80016cd0:
        return G3Function80016CD0(object, command, guest_stack);
    case 0x80016d44:
        return G3Function80016D44(object, command, guest_stack);
    case 0x80016d80:
        return G3Function80016D80(object, command, guest_stack);
    case 0x80016dbc:
        return G3Function80016DBC(object, command, guest_stack);
    case 0x80016de4:
        return G3Function80016DE4(object, command, guest_stack);
    case 0x80016e20:
        return G3Function80016E20(object, command);
    case 0x80016e64:
        return G3Function80016E64(object, command);
    case 0x80016ea8:
        return G3Function80016EA8(object, command, guest_stack);
    case 0x80016ecc:
        return G3Function80016ECC();
    case 0x80016ed4:
        return G3Function80016ED4();
    case 0x80016edc:
        return G3Function80016EDC(object, command, guest_stack);
    case 0x80016f00:
        return G3Function80016F00();
    case 0x80016f24:
        return G3Function80016F24();
    case 0x8001722c:
        return G3Function8001722C(object, command, guest_stack);
    case 0x8001725c:
        return G3Function8001725C(object, command, guest_stack);
    case 0x8001727c:
        return G3Function8001727C(object, command, guest_stack);
    case 0x800172dc:
        return G3Function800172DC(object, command, guest_stack);
    case 0x80017384:
        return G3Function80017384(object, command);
    case 0x800173e0:
        return G3Function800173E0(object, command);
    case 0x80017410:
        return G3Function80017410(object, command, guest_stack);
    case 0x800174b4:
        return G3Function800174B4(object, command);
    case 0x800174d8:
        return G3Function800174D8(object, command, guest_stack);
    case 0x80017518:
        return G3Function80017518(object, command, guest_stack);
    case 0x80017598:
        return G3Function80017598();
    case 0x800175a0:
        return G3Function800175A0();
    case 0x800175a8:
        return G3Function800175A8();
    case 0x800175b0:
        return G3Function800175B0(object, command, guest_stack);
    case 0x80017618:
        return G3Function80017618(object, command, guest_stack);
    case 0x80017958:
        return G3Function80017958(object, command, guest_stack);
    case 0x80017d00:
        return G3Function80017D00(object, command);
    case 0x80017d80:
        return G3Function80017D80(object, command);
    case 0x80017dc0:
        return G3Function80017DC0();
    case 0x80017dc8:
        return G3Function80017DC8();
    case 0x80017dd0:
        return G3Function80017DD0(object, command, guest_stack);
    case 0x80017df8:
        return G3Function80017DF8(object, command, guest_stack);
    case 0x80017e20:
        return G3Function80017E20();
    case 0x80017e40:
        return G3Function80017E40();
    case 0x80017e60:
        return G3Function80017E60();
    case 0x80017e80:
        return G3Function80017E80(object, command, guest_stack);
    case 0x80017ee0:
        return G3Function80017EE0(object, command);
    case 0x80017f40:
        return G3Function80017F40(object, command, guest_stack);
    case 0x80017fa4:
        return G3Function80017FA4(object, command);
    case 0x80017fd4:
        return G3Function80017FD4(object, command);
    case 0x80017ff8:
        return G3Function80017FF8(object, command);
    case 0x8001801c:
        return G3Function8001801C(object, command);
    case 0x80018034:
        return G3Function80018034(object, command);
    case 0x80018078:
        return G3Function80018078(object, command);
    case 0x80018108:
        return G3Function80018108(object, command);
    case 0x800181ac:
        return G3Function800181AC(object, command);
    case 0x8001823c:
        return G3Function8001823C(object, command);
    case 0x80018280:
        return G3Function80018280(object, command);
    case 0x8001829c:
        return G3Function8001829C(object, command, guest_stack);
    case 0x800182bc:
        return G3Function800182BC(object, command, guest_stack);
    case 0x80018314:
        return G3Function80018314(object, command, guest_stack);
    case 0x8001836c:
        return G3Function8001836C(object, command);
    case 0x800183e8:
        return G3Function800183E8(object, command);
    case 0x80018474:
        return G3Function80018474();
    case 0x8001847c:
        return G3Function8001847C(object, command);
    case 0x800184cc:
        return G3Function800184CC(object, command);
    case 0x80018508:
        return G3Function80018508(object, command);
    case 0x80018558:
        return G3Function80018558(object, command);
    case 0x800185a0:
        return G3Function800185A0(object, command);
    case 0x800185e4:
        return G3Function800185E4(object, command);
    case 0x80018608:
        return G3Function80018608(object, command, guest_stack);
    case 0x8001863c:
        return G3Function8001863C(object, command);
    case 0x80018688:
        return G3Function80018688(object, command);
    case 0x800186dc:
        return G3Function800186DC(object, command, guest_stack);
    case 0x8001877c:
        return G3Function8001877C(object, command);
    case 0x800187b0:
        return G3Function800187B0();
    case 0x800187dc:
        return G3Function800187DC(object, command, guest_stack);
    case 0x8001887c:
        return G3Function8001887C(object, command);
    case 0x800188e4:
        return G3Function800188E4(guest_stack);
    case 0x8001893c:
        return G3Function8001893C();
    case 0x80018980:
        return G3Function80018980(guest_stack);
    case 0x800189d4:
        return G3Function800189D4(object, command, guest_stack);
    case 0x80018a40:
        return G3Function80018A40(object, command, guest_stack);
    case 0x80018a84:
        return G3Function80018A84(object, command, guest_stack);
    case 0x80018ad4:
        return G3Function80018AD4(guest_stack);
    case 0x80018b1c:
        return G3Function80018B1C(object, command, guest_stack);
    case 0x80018b90:
        return G3Function80018B90(object, command, guest_stack);
    case 0x80018c00:
        return G3Function80018C00(object, command, guest_stack);
    case 0x80018c7c:
        return G3Function80018C7C(object, command);
    case 0x80018ca4:
        return G3Function80018CA4();
    case 0x80018cc4:
        return G3Function80018CC4(object, command, guest_stack);
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved script command target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

static uint32 G3DispatchFailure(const char *operation, uint32 target)
{
    {
#ifndef NDEBUG
    fprintf(stderr, "G3 unresolved %s target %08X\n", operation, (unsigned int)target);
#endif
    }
    abort();
}

uint32 G3DispatchModelDecoder(uint32 target, uint32 reader, uint32 destination, uint32 selector, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80030e9c:
        return G3Function80030E9C(reader, destination);
    case 0x80030ec8:
        return G3Function80030EC8(reader, destination);
    case 0x80030f00:
        return G3Function80030F00(reader, destination);
    case 0x80030f58:
        return G3Function80030F58(reader, destination);
    default:
        return G3DispatchFailure("model decoder", target);
    }
}

uint32 G3DispatchChannelValue(uint32 target, uint32 context, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80030bdc:
        return G3Function80030BDC(context);
    case 0x80030c10:
        return G3Function80030C10(context);
    case 0x80030c2c:
        return G3Function80030C2C(context);
    case 0x80030d8c:
        return G3Function80030D8C(context);
    default:
        return G3DispatchFailure("channel value", target);
    }
}

uint32 G3DispatchNearestPoint(uint32 target, uint32 output, uint32 start, uint32 end, uint32 point, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8005fc70:
        return G3Function8005FC70(output, start, end, point);
    default:
        return G3DispatchFailure("nearest point", target);
    }
}

uint32 G3DispatchLinePlane(uint32 target, uint32 output, uint32 start, uint32 end, uint32 normal, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8005f518:
        return G3Function8005F518(output, start, end, normal, guest_stack);
    default:
        return G3DispatchFailure("line plane", target);
    }
}

uint32 G3DispatchPolygonRender(uint32 target, uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80063c6c:
        return G3Function80063C6C(model, vertices, packets, order_table, guest_stack);
    default:
        return G3DispatchFailure("polygon renderer", target);
    }
}

void G3DispatchSceneRender(uint32 target, uint32 scene, uint32 camera, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8001ce1c:
        (void)G3Function8001CE1C(scene, camera, packets, order_table);
        return;
    default:
        (void)G3DispatchFailure("scene renderer", target);
    }
}

uint32 G3DispatchPrimitiveRender(uint32 target, uint32 model, uint32 object, uint32 vertices, uint32 matrix, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80019fc8:
        return G3Function80019FC8(model, object, vertices, matrix, packets, order_table, guest_stack);
    default:
        return G3DispatchFailure("primitive renderer", target);
    }
}

sint32 G3NativeMainEvent(void)
{
    return (sint32)G3EventCallback8002EBE0();
}

sint32 G3NativeClockEvent(void)
{
    G3OnRootCounter8001947C();
    return 0;
}

// Preserve the original fifth word already stored by the two-phase owner
uint32 G3DispatchObjectEvent(uint32 target, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 phase, uint32 guest_stack)
{
    return G3DispatchStateEvent(target, object, game, message, payload, guest_stack);
}

// Forward all four carriers and the unchanged face-initializer outgoing homes
uint32 G3DispatchFaceInitializer(uint32 target, uint32 effect, uint32 callback, uint32 owner, uint32 object, uint32 guest_stack)
{
    return G3DispatchEffectInitializer(target, effect, callback, owner, object, guest_stack);
}

// The original audio callback uses the same four-word call carrier contract
uint32 G3DispatchAudioParameter(uint32 target, uint32 owner, uint32 channel, uint32 parameter, uint32 value, uint32 guest_stack)
{
    return G3DispatchStateEvent(target, owner, channel, parameter, value, guest_stack);
}

void G3DispatchAudioFade(uint32 target, uint32 guest_stack)
{
    switch (target)
    {
    case 0x80011f98:
        (void)G3Function80011F98();
        return;
    case 0x800126bc:
        (void)G3Function800126BC();
        return;
    case 0x800126c4:
        (void)G3Function800126C4();
        return;
    case 0x800126cc:
        (void)G3Function800126CC();
        return;
    case 0x800126d4:
        (void)G3Function800126D4();
        return;
    case 0x800126dc:
        (void)G3Function800126DC();
        return;
    case 0x800126ec:
        (void)G3Function800126EC();
        return;
    case 0x800126f4:
        (void)G3Function800126F4();
        return;
    case 0x8001294c:
        (void)G3Function8001294C();
        return;
    case 0x80012a14:
        (void)G3Function80012A14();
        return;
    case 0x80012f94:
        (void)G3Function80012F94();
        return;
    case 0x80012f9c:
        (void)G3Function80012F9C();
        return;
    case 0x80014900:
        (void)G3Function80014900();
        return;
    case 0x800154d4:
        (void)G3Function800154D4();
        return;
    case 0x800154dc:
        (void)G3Function800154DC();
        return;
    case 0x800155d4:
        (void)G3Function800155D4();
        return;
    case 0x8001589c:
        (void)G3Function8001589C();
        return;
    case 0x800158a4:
        (void)G3Function800158A4();
        return;
    case 0x80015cfc:
        (void)G3Function80015CFC();
        return;
    case 0x80016248:
        (void)G3Function80016248();
        return;
    case 0x80016820:
        (void)G3Function80016820();
        return;
    case 0x80016828:
        (void)G3Function80016828();
        return;
    case 0x80016888:
        (void)G3Function80016888();
        return;
    case 0x80016ecc:
        (void)G3Function80016ECC();
        return;
    case 0x80016ed4:
        (void)G3Function80016ED4();
        return;
    case 0x80016f00:
        (void)G3Function80016F00();
        return;
    case 0x80017598:
        (void)G3Function80017598();
        return;
    case 0x800175a0:
        (void)G3Function800175A0();
        return;
    case 0x800175a8:
        (void)G3Function800175A8();
        return;
    case 0x80017dc0:
        (void)G3Function80017DC0();
        return;
    case 0x80017dc8:
        (void)G3Function80017DC8();
        return;
    case 0x80017e20:
        (void)G3Function80017E20();
        return;
    case 0x80017e40:
        (void)G3Function80017E40();
        return;
    case 0x80017e60:
        (void)G3Function80017E60();
        return;
    case 0x80018474:
        (void)G3Function80018474();
        return;
    case 0x800187b0:
        (void)G3Function800187B0();
        return;
    case 0x8001893c:
        (void)G3Function8001893C();
        return;
    case 0x80018ca4:
        (void)G3Function80018CA4();
        return;
    case 0x80019440:
        (void)G3ClearBss80019440();
        return;
    case 0x8001947c:
        (void)G3OnRootCounter8001947C();
        return;
    case 0x80020a04:
        (void)G3Function80020A04();
        return;
    case 0x80021aac:
        (void)G3Function80021AAC();
        return;
    case 0x80022dec:
        (void)G3Function80022DEC();
        return;
    case 0x80026438:
        (void)G3Function80026438();
        return;
    case 0x80026904:
        (void)G3Function80026904();
        return;
    case 0x80027fdc:
        (void)G3Function80027FDC();
        return;
    case 0x80028498:
        (void)G3Function80028498();
        return;
    case 0x80028524:
        (void)G3Function80028524();
        return;
    case 0x80028f78:
        (void)G3Function80028F78();
        return;
    case 0x8002b484:
        (void)G3Function8002B484();
        return;
    case 0x8002daec:
        (void)G3PresentDisplay8002DAEC();
        return;
    case 0x8002dcc4:
        (void)G3InitializeClock8002DCC4();
        return;
    case 0x8002dd34:
        (void)G3OnVSync8002DD34();
        return;
    case 0x8002dda8:
        (void)G3OnDrawComplete8002DDA8();
        return;
    case 0x8002e218:
        (void)G3Function8002E218();
        return;
    case 0x8002ebe0:
        (void)G3EventCallback8002EBE0();
        return;
    case 0x8002ed54:
        (void)G3Function8002ED54();
        return;
    case 0x80030414:
        (void)G3Function80030414();
        return;
    case 0x80033950:
        (void)G3Function80033950();
        return;
    case 0x8003525c:
        (void)G3Function8003525C();
        return;
    case 0x80035348:
        (void)G3Function80035348();
        return;
    case 0x8003556c:
        (void)G3Function8003556C();
        return;
    case 0x800356e4:
        (void)G3Function800356E4();
        return;
    case 0x80035a80:
        (void)G3Function80035A80();
        return;
    case 0x80035aa4:
        (void)G3Function80035AA4();
        return;
    case 0x80035ac8:
        (void)G3Function80035AC8();
        return;
    case 0x80036158:
        (void)G3Function80036158();
        return;
    case 0x800361d4:
        (void)G3Function800361D4();
        return;
    case 0x8003ac34:
        (void)G3Function8003AC34();
        return;
    case 0x8003e30c:
        (void)G3Function8003E30C();
        return;
    case 0x8003eb28:
        (void)G3Function8003EB28();
        return;
    case 0x8003ece0:
        (void)G3Function8003ECE0();
        return;
    case 0x8003ef1c:
        (void)G3Function8003EF1C();
        return;
    case 0x80046ad0:
        (void)G3Function80046AD0();
        return;
    case 0x80048130:
        (void)G3MemPackInit80048130();
        return;
    case 0x8004829c:
        (void)G3MemPackPush8004829C();
        return;
    case 0x80048320:
        (void)G3MemPackPop80048320();
        return;
    case 0x800487dc:
        (void)G3Function800487DC();
        return;
    case 0x80049040:
        (void)G3Function80049040();
        return;
    case 0x80049060:
        (void)G3Function80049060();
        return;
    case 0x80049080:
        (void)G3Function80049080();
        return;
    case 0x800490bc:
        (void)G3Function800490BC();
        return;
    case 0x80049144:
        (void)G3Function80049144();
        return;
    case 0x8004917c:
        (void)G3Function8004917C();
        return;
    case 0x80049434:
        (void)G3Function80049434();
        return;
    case 0x80049a60:
        (void)G3Function80049A60();
        return;
    case 0x80049b20:
        (void)G3Function80049B20();
        return;
    case 0x80049b44:
        (void)G3Function80049B44();
        return;
    case 0x80049df4:
        (void)G3Function80049DF4();
        return;
    case 0x8004aa4c:
        (void)G3Function8004AA4C();
        return;
    case 0x8004aaac:
        (void)G3Function8004AAAC();
        return;
    case 0x8004aafc:
        (void)G3Function8004AAFC();
        return;
    case 0x8004ac30:
        (void)G3Function8004AC30();
        return;
    case 0x8004adc4:
        (void)G3Function8004ADC4();
        return;
    case 0x8004adcc:
        (void)G3Function8004ADCC();
        return;
    case 0x8004ae40:
        (void)G3Function8004AE40();
        return;
    case 0x8004af30:
        (void)G3Function8004AF30();
        return;
    case 0x8004af38:
        (void)G3Function8004AF38();
        return;
    case 0x8004b1a4:
        (void)G3Function8004B1A4();
        return;
    case 0x8004b1ac:
        (void)G3Function8004B1AC();
        return;
    case 0x8004b1b4:
        (void)G3Function8004B1B4();
        return;
    case 0x8004b1bc:
        (void)G3Function8004B1BC();
        return;
    case 0x8004b1dc:
        (void)G3Function8004B1DC();
        return;
    case 0x8004b1e4:
        (void)G3Function8004B1E4();
        return;
    case 0x8004b1ec:
        (void)G3Function8004B1EC();
        return;
    case 0x8004b1f4:
        (void)G3Function8004B1F4();
        return;
    case 0x8004b1fc:
        (void)G3Function8004B1FC();
        return;
    case 0x8004b204:
        (void)G3Function8004B204();
        return;
    case 0x8004b20c:
        (void)G3Function8004B20C();
        return;
    case 0x8004b3a0:
        (void)G3Function8004B3A0();
        return;
    case 0x8004b3dc:
        (void)G3Function8004B3DC();
        return;
    case 0x8004b3e4:
        (void)G3Function8004B3E4();
        return;
    case 0x8004b658:
        (void)G3Function8004B658();
        return;
    case 0x8004b660:
        (void)G3Function8004B660();
        return;
    case 0x8004b668:
        (void)G3Function8004B668();
        return;
    case 0x8004b670:
        (void)G3Function8004B670();
        return;
    case 0x8004bc18:
        (void)G3Function8004BC18();
        return;
    case 0x8004c380:
        (void)G3Function8004C380();
        return;
    case 0x8004d9b0:
        (void)G3Function8004D9B0();
        return;
    case 0x8004d9e0:
        (void)G3Function8004D9E0();
        return;
    case 0x8004dae4:
        (void)G3Function8004DAE4();
        return;
    case 0x8004dafc:
        (void)G3Function8004DAFC();
        return;
    case 0x8004db1c:
        (void)G3Function8004DB1C();
        return;
    case 0x8004dc44:
        (void)G3Function8004DC44();
        return;
    case 0x8004df3c:
        (void)G3Function8004DF3C();
        return;
    case 0x8004f5d4:
        (void)G3Function8004F5D4();
        return;
    case 0x80051864:
        (void)G3Function80051864();
        return;
    case 0x800520cc:
        (void)G3Function800520CC();
        return;
    case 0x800524e4:
        (void)G3Function800524E4();
        return;
    case 0x80055194:
        (void)G3Function80055194();
        return;
    case 0x80055580:
        (void)G3Function80055580();
        return;
    case 0x80056064:
        (void)G3Function80056064();
        return;
    case 0x80056464:
        (void)G3Function80056464();
        return;
    case 0x80056924:
        (void)G3Function80056924();
        return;
    case 0x80058b54:
        (void)G3Function80058B54();
        return;
    case 0x80060044:
        (void)G3Function80060044();
        return;
    case 0x80064de8:
        (void)G3CdClearPending80064DE8();
        return;
    case 0x800657c0:
        (void)G3Function800657C0();
        return;
    case 0x80066e98:
        (void)G3GetCardCoreSize80066E98();
        return;
    case 0x80067940:
        (void)G3Function80067940();
        return;
    case 0x80067c7c:
        (void)G3Function80067C7C();
        return;
    case 0x80067f64:
        (void)G3Function80067F64();
        return;
    case 0x8006898c:
        (void)G3GetMemoryCardSize8006898C();
        return;
    case 0x800697d0:
        (void)G3GetSaveContextSize800697D0();
        return;
    case 0x8006a624:
        (void)G3Function8006A624();
        return;
    case 0x8006a6bc:
        (void)G3Function8006A6BC();
        return;
    case 0x8006b2f4:
        (void)G3SaveDataSize8006B2F4();
        return;
    case 0x8006d4c8:
        (void)G3Function8006D4C8();
        return;
    case 0x8006db98:
        (void)G3Function8006DB98();
        return;
    case 0x8006dc34:
        (void)G3Function8006DC34();
        return;
    case 0x800752b8:
        (void)G3Function800752B8();
        return;
    case 0x800752d8:
        (void)G3Function800752D8();
        return;
    case 0x800760c0:
        (void)G3Function800760C0();
        return;
    case 0x80081010:
        (void)G3Function80081010();
        return;
    case 0x8008101c:
        (void)G3Function8008101C();
        return;
    case 0x8008102c:
        (void)G3Function8008102C();
        return;
    case 0x800810fc:
        (void)G3Function800810FC();
        return;
    case 0x80081154:
        (void)G3Function80081154();
        return;
    case 0x80081c58:
        (void)G3Function80081C58();
        return;
    case 0x80094f54:
        (void)G3ConsoleFlush80094F54();
        return;
    default:
        break;
    }
    uint32 identity_id;
    uint32 module_base;
    uint32 offset;
    if (G3LevelCodeResolve(target, &identity_id, &module_base, &offset))
    {
        if (identity_id == 15 && offset == 0x710)
        {
            (void)G3SnowWeaponContact();
            return;
        }
        if (identity_id == 39 && offset == 0x3d8)
        {
            (void)G3MstrdbgNoop03D8();
            return;
        }
        if (identity_id == 39 && offset == 0x8bc)
        {
            (void)G3MstrdbgNoop08BC();
            return;
        }
        if (identity_id == 39 && offset == 0x7e48)
        {
            (void)G3MstrdbgNoop7E48();
            return;
        }
        if (identity_id == 39 && offset == 0x7e7c)
        {
            (void)G3MstrdbgAccept7E7C();
            return;
        }
        if (identity_id == 56 && offset == 0xb10)
        {
            (void)G3WaterFanCollide();
            return;
        }
        if (identity_id == 60 && offset == 0x59c)
        {
            (void)G3HitchupCollide();
            return;
        }
        if (identity_id == 65 && offset == 0x604)
        {
            (void)G3MonsterNoop0604();
            return;
        }
        if (identity_id == 65 && offset == 0xa30)
        {
            (void)G3MonsterNoop0A30();
            return;
        }
        if (identity_id == 65 && offset == 0x6550)
        {
            (void)G3MonsterConstant6550();
            return;
        }
        if (identity_id == 65 && offset == 0x72d0)
        {
            (void)G3MonsterNoop72D0();
            return;
        }
        if (identity_id == 65 && offset == 0x7304)
        {
            (void)G3MonsterConstant7304();
            return;
        }
        if (identity_id == 65 && offset == 0x7bc8)
        {
            (void)G3MonsterNoop7BC8();
            return;
        }
        if (identity_id == 67 && offset == 0x55c)
        {
            (void)G3ChunkNoOperation();
            return;
        }
        if (identity_id == 77 && offset == 0x12e4)
        {
            (void)G3FanFinish();
            return;
        }
    }
    (void)G3DispatchFailure("audio fade without reviewed zero-argument ABI", target);
}

extern "C" void g3_main_8002ebe8(uint32 guest_stack, uint32 guest_return)
{
    (void)G3Main8002EBE8(guest_stack);
}

uint32 G3StateWait8002E8F4(uint32 state)
{
    FUNCTION_MARKER(0x8002e8f4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    if (r_u32(game + 0x154) >= 151)
    {
        w_u32(state, 6);
        w_u32(state + 8, 0);
    }
    return 6;
}

uint32 G3StateExit8002EBCC(uint32 state)
{
    FUNCTION_MARKER(0x8002ebcc, "SLUS_008.06");
    w_u32(state + 0x18, 1);
    return 1;
}

void G3StateIdle8002EBD8(void)
{
    FUNCTION_MARKER(0x8002ebd8, "SLUS_008.06");
}

void G3DispatchMainState(uint32 target, uint32 state, uint32 guest_stack)
{
    switch (target)
    {
    case 0x8002e218: (void)G3Function8002E218(); return;
    case 0x8002e220: (void)G3StateStartup8002E220(state, guest_stack); return;
    case 0x8002e370: (void)G3StateRun8002E370(state, 0, 0, guest_stack); return;
    case 0x8002e61c: (void)G3StateLoad8002E61C(state, guest_stack); return;
    case 0x8002e7ec: (void)G3StateWarning8002E7EC(state, guest_stack); return;
    case 0x8002e8f4: (void)G3StateWait8002E8F4(state); return;
    case 0x8002e924: (void)G3StateMovie8002E924(state, guest_stack); return;
    case 0x8002ebcc: (void)G3StateExit8002EBCC(state); return;
    case 0x8002ebd8: G3StateIdle8002EBD8(); return;
    default: (void)G3DispatchFailure("main state", target); return;
    }
}

uint32 G3DispatchAudioCommand(uint32 target, uint32 command)
{
    switch (target)
    {
    case 0x8004c154: return G3Function8004C154(command);
    case 0x8004c2c0: return G3Function8004C2C0(command);
    case 0x8004c42c: return G3Function8004C42C(command);
    case 0x8004c78c: return G3Function8004C78C(command);
    case 0x8004c380: return G3Function8004C380();
    case 0x8004cbf4: return G3Function8004CBF4(command);
    case 0x8004cc40: return G3Function8004CC40(command);
    case 0x8004cccc: return G3Function8004CCCC(command);
    case 0x8004ccec: return G3Function8004CCEC(command);
    default: return G3DispatchFailure("audio command", target);
    }
}

/* Use the approved dedicated audio workspace */
sint32 G3NativeAudioEvent(void)
{
    return (sint32)G3Function80048870(0x8009b5b8);
}

uint32 G3DispatchAllocatedVoice(uint32 target, uint32 mask)
{
    switch (target)
    {
    case 0x80035250: return G3Function80035250(mask);
    default: return G3DispatchFailure("allocated voice", target);
    }
}
