// ==========================================================================
// Dedmonwakeen's Raid DPS/TPS Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.hpp"

#ifndef SC_WARLOCK_HPP
#define SC_WARLOCK_HPP

#if SC_WARLOCK == 1

struct warlock_targetdata_t : public targetdata_t
{
  dot_t*  dots_corruption;
  dot_t*  dots_unstable_affliction;
  dot_t*  dots_agony;
  dot_t*  dots_doom;
  dot_t*  dots_immolate;
  dot_t*  dots_drain_life;
  dot_t*  dots_drain_soul;
  dot_t*  dots_shadowflame;
  dot_t*  dots_burning_embers;
  dot_t*  dots_malefic_grasp;

  buff_t* debuffs_haunt;

  int affliction_effects();
  int active_dots();

  warlock_targetdata_t( warlock_t* source, player_t* target );
};

struct warlock_pet_t;
struct warlock_main_pet_t;
struct warlock_guardian_pet_t;

struct warlock_t : public player_t
{
  // Active Pet
  struct pets_t
  {
    warlock_main_pet_t* active;
    pet_t* ebon_imp;
  } pets;

  // Buffs
  struct buffs_t
  {
    buff_t* backdraft;
    buff_t* decimation;
    buff_t* metamorphosis;
    buff_t* molten_core;
    buff_t* soulburn;
    buff_t* bane_of_havoc;
    buff_t* tier13_4pc_caster;
  } buffs;

  // Cooldowns
  struct cooldowns_t
  {
    cooldown_t* metamorphosis;
    cooldown_t* infernal;
    cooldown_t* doomguard;
  } cooldowns;

  // Talents

  struct talents_t
  {
    const spell_data_t* dark_regeneration;
    const spell_data_t* soul_leech;
    const spell_data_t* harvest_life;

    const spell_data_t* howl_of_terror;
    const spell_data_t* mortal_coil;
    const spell_data_t* shadowfury;

    const spell_data_t* soul_link;
    const spell_data_t* sacrificial_pact;
    const spell_data_t* dark_bargain;

    const spell_data_t* blood_fear;
    const spell_data_t* burning_rush;
    const spell_data_t* unbound_will;

    const spell_data_t* grimoire_of_supremacy;
    const spell_data_t* grimoire_of_service;
    const spell_data_t* grimoire_of_sacrifice;

    const spell_data_t* archimondes_vengeance;
    const spell_data_t* kiljaedens_cunning;
    const spell_data_t* mannoroths_fury;
  } talents;

  // Specialization Spells
  struct specs_t
  {
    // General   
    const spell_data_t* nethermancy;

    // Affliction
    const spell_data_t* nightfall;

    // Demonology
    const spell_data_t* decimation;

    // Destruction
    const spell_data_t* burning_embers;

  } spec;

  struct mastery_spells_t
  {
    const spell_data_t* potent_afflictions;
    const spell_data_t* master_demonologist;
    const spell_data_t* fiery_apocalypse;
  } mastery_spells;

  std::string dark_intent_target_str;

  // Gains
  struct gains_t
  {
    gain_t* fel_armor;
    gain_t* felhunter;
    gain_t* life_tap;
    gain_t* soul_leech;
    gain_t* mana_feed;
    gain_t* tier13_4pc;
    gain_t* nightfall;
    gain_t* drain_soul;
  } gains;

  // Uptimes
  struct benefits_t
  {
    benefit_t* backdraft[ 4 ];
  } benefits;

  // Procs
  struct procs_t
  {
    proc_t* shadow_trance;
  } procs;

  // Random Number Generators
  struct rngs_t
  {
    rng_t* nightfall;
  } rngs;

  // Spells
  spell_t* spells_burning_embers;

  struct glyphs_t
  {
    const spell_data_t* metamorphosis;
    const spell_data_t* life_tap;
    const spell_data_t* shadow_bolt;
    const spell_data_t* chaos_bolt;
    const spell_data_t* conflagrate;
    const spell_data_t* corruption;
    const spell_data_t* bane_of_agony;
    const spell_data_t* felguard;
    const spell_data_t* haunt;
    const spell_data_t* immolate;
    const spell_data_t* imp;
    const spell_data_t* incinerate;
    const spell_data_t* lash_of_pain;
    const spell_data_t* shadowburn;
    const spell_data_t* unstable_affliction;
    // MoP
    const spell_data_t* burning_embers;
    const spell_data_t* demon_hunting;
    const spell_data_t* demon_training;
    const spell_data_t* doom;
    const spell_data_t* soul_shards;
  };
  glyphs_t glyphs;

  int use_pre_soulburn;

  warlock_pet_t* active_pet;

  warlock_t( sim_t* sim, const std::string& name, race_type_e r = RACE_UNDEAD );


  // Character Definition
  virtual warlock_targetdata_t* new_targetdata( player_t* target )
  { return new warlock_targetdata_t( this, target ); }
  virtual void      init_spells();
  virtual void      init_base();
  virtual void      init_scaling();
  virtual void      init_buffs();
  virtual void      init_values();
  virtual void      init_gains();
  virtual void      init_benefits();
  virtual void      init_procs();
  virtual void      init_rng();
  virtual void      init_actions();
  virtual void      init_resources( bool force );
  virtual void      reset();
  virtual void      create_options();
  virtual action_t* create_action( const std::string& name, const std::string& options );
  buff_t*   create_buff( const std::string& name );
  buff_t*   create_buff( int id, const std::string& token );
  virtual pet_t*    create_pet   ( const std::string& name, const std::string& type = std::string() );
  virtual void      create_pets();
  virtual bool      create_profile( std::string& profile_str, save_type_e=SAVE_ALL, bool save_html=false );
  virtual void      copy_from( player_t* source );
  virtual int       decode_set( const item_t& ) const;
  virtual resource_type_e primary_resource() const { return RESOURCE_MANA; }
  virtual role_type_e primary_role() const     { return ROLE_SPELL; }
  virtual double    composite_spell_power_multiplier() const;
  virtual double    matching_gear_multiplier( attribute_type_e attr ) const;
  virtual double composite_player_multiplier( school_type_e school, const action_t* a ) const;
};

// ==========================================================================
// Warlock Pet
// ==========================================================================

namespace pet_stats{
struct _weapon_list_t;
}
struct warlock_pet_t : public pet_t
{
  double damage_modifier;
  int stats_avaiable;
  int stats2_avaiable;

  gain_t* gains_mana_feed;
  proc_t* procs_mana_feed;

  double get_attribute_base( int level, int stat_type_e, pet_type_e pet_type );
private:
  const pet_stats::_weapon_list_t* get_weapon( pet_type_e pet_type );
public:
  double get_weapon_min( int level, pet_type_e pet_type );
  double get_weapon_max( int level, pet_type_e pet_type );
  timespan_t get_weapon_swing_time( int level, pet_type_e pet_type );
  warlock_pet_t( sim_t* sim, warlock_t* owner, const std::string& pet_name, pet_type_e pt, bool guardian = false );
  virtual bool ooc_buffs() { return true; }
  virtual void init_base();
  virtual void init_resources( bool force );
  virtual void schedule_ready( timespan_t delta_time=timespan_t::zero(),
                               bool   waiting=false );
  virtual void summon( timespan_t duration=timespan_t::zero() );
  virtual void dismiss();
  virtual double composite_spell_haste() const;
  virtual double composite_attack_haste() const;
  virtual double composite_spell_power( school_type_e school ) const;
  virtual double composite_attack_power() const;
  virtual double composite_attack_crit( const weapon_t* ) const;
  virtual double composite_spell_crit() const;
  warlock_t* o() const
  { return static_cast<warlock_t*>( owner ); }
};

// ==========================================================================
// Warlock Main Pet
// ==========================================================================

struct warlock_main_pet_t : public warlock_pet_t
{
  warlock_main_pet_t( sim_t* sim, warlock_t* owner, const std::string& pet_name, pet_type_e pt );
  virtual void summon( timespan_t duration=timespan_t::zero() );
  virtual void dismiss();
  virtual double composite_attack_expertise( const weapon_t* ) const;
  virtual resource_type_e primary_resource() const;
  virtual double composite_mp5() const;
  virtual double composite_player_multiplier( school_type_e school, const action_t* a ) const;
};

// ==========================================================================
// Warlock Guardian Pet
// ==========================================================================

struct warlock_guardian_pet_t : public warlock_pet_t
{
  double snapshot_crit, snapshot_haste, snapshot_sp, snapshot_mastery;

  warlock_guardian_pet_t( sim_t* sim, warlock_t* owner, const std::string& pet_name, pet_type_e pt );
  virtual void summon( timespan_t duration=timespan_t::zero() );
  virtual double composite_attack_crit( const weapon_t* ) const;
  virtual double composite_attack_expertise( const weapon_t* ) const;
  virtual double composite_attack_haste() const;
  virtual double composite_attack_hit() const;
  virtual double composite_attack_power() const;
  virtual double composite_spell_crit() const;
  virtual double composite_spell_haste() const;
  virtual double composite_spell_power( school_type_e school ) const;
};

// ==========================================================================
// Pet Imp
// ==========================================================================

struct imp_pet_t : public warlock_main_pet_t
{
  imp_pet_t( sim_t* sim, warlock_t* owner );
  virtual void init_base();
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
};

// ==========================================================================
// Pet Felguard
// ==========================================================================

struct felguard_pet_t : public warlock_main_pet_t
{
  felguard_pet_t( sim_t* sim, warlock_t* owner );
  virtual void init_base();
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
};

// ==========================================================================
// Pet Felhunter
// ==========================================================================

struct felhunter_pet_t : public warlock_main_pet_t
{
  felhunter_pet_t( sim_t* sim, warlock_t* owner );
  virtual void init_base();
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
  virtual void summon( timespan_t duration=timespan_t::zero() );
  virtual void dismiss();
};

// ==========================================================================
// Pet Succubus
// ==========================================================================

struct succubus_pet_t : public warlock_main_pet_t
{
  succubus_pet_t( sim_t* sim, warlock_t* owner );
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
};

// ==========================================================================
// Pet Voidwalker
// ==========================================================================

struct voidwalker_pet_t : public warlock_main_pet_t
{
  voidwalker_pet_t( sim_t* sim, warlock_t* owner );
  virtual void init_base();
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
};

// ==========================================================================
// Pet Infernal
// ==========================================================================

struct infernal_pet_t : public warlock_guardian_pet_t
{
  infernal_pet_t( sim_t* sim, warlock_t* owner );
  virtual void init_base();
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
};

// ==========================================================================
// Pet Doomguard
// ==========================================================================

struct doomguard_pet_t : public warlock_guardian_pet_t
{
  doomguard_pet_t( sim_t* sim, warlock_t* owner );
  virtual void init_base();
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str );
  virtual double composite_player_multiplier( school_type_e school, const action_t* a ) const;
};

// ==========================================================================
// Pet Ebon Imp
// ==========================================================================

struct ebon_imp_pet_t : public warlock_guardian_pet_t
{
  ebon_imp_pet_t( sim_t* sim, warlock_t* owner );
  virtual timespan_t available() const;
  virtual double composite_attack_power() const;
  virtual void init_base();
};

#endif

#endif
