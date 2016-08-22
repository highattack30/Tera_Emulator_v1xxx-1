#ifndef ITEMENUM_H
#define ITEMENUM_H


enum ItemCategory
{
	combat,
	magical,
	custormaize,
	pkitem,
	medal,
	service,
	earring,
	necklace,
	ring,
	skillbook,
	token,
	order,
	etc,
	petAdult,
	petOrb,
	dyeItem,
	dyeRaw,
	dyeRecipe,
	dual,
	lance,
	twohand,
	axe,
	circle,
	bow,
	staff,
	rod,
	accessoryHair,
	weaponMaterial,
	armorMaterial,
	keyItem,
	generalMaterial,
	fiber,
	metal,
	alchemy,
	leather,
	alchemyMaterial,
	fireworkMaterial,
	weaponComponent,
	armorComponent,
	bodyMail,
	handMail,
	feetMail,
	bodyLeather,
	handLeather,
	feetLeather,
	bodyRobe,
	handRobe,
	feetRobe,
	recipe,
	quest,
	document,
	crestPoint,
	crest,
	charm,
	extractRecipe,
	accessoryFace,
	style_face,
	style_hair,
	style_dual,
	style_lance,
	style_twohand,
	style_axe,
	style_circle,
	style_bow,
	style_staff,
	style_rod
};

enum ItemResourceType
{
	small
};

enum CombatItemType
{
	DISPOSAL,
	CUSTOM,
	NO_COMBAT,
	CHANGE_NAME,
	EQUIP_ACCESSORY,
	SKILLBOOK,
	DUNGEON_COIN,
	CURSED_DREAM_PIECE,
	BLESSED_DREAM_PIECE,
	UNIDENTIFY_SCROLL,
	CURSE_REMOVE_SCROLL,
	IDENTIFY_SCROLL,
	CHANGE_COLOR_ITEM,
	MIX_DISPOSAL,
	CHANGE_RACE,
	CHANGE_GENDER,
	CHANGE_LOOKS,
	EQUIP_WEAPON,
	START_GUILD_WAR,
	GIVE_UP_GUILD_WAR,
	CHRONO_SCROLL,
	DUNGEON_RESET_SCROLL,
	RESET_SCROLL,
	IMMEDIATE,
	EQUIP_ARMOR_BODY,
	EQUIP_ARMOR_ARM,
	EQUIP_ARMOR_LEG,
	DOCUMENT,
	CREST_RESET,
	CREST_POINT,
	CREST,
	RECIPE,
	EQUIP_STYLE_ACCESSORY,
	EQUIP_STYLE_WEAPON
};

enum ItemType : int
{

};

enum ItemRareGrade : int
{
	NORMAL = 0,
	UNCOMMON = 1,
	RARE = 2,
	SUPERIOR = 3
};

enum ItemGrade : int
{
	INORMAL = 0,
	MASTERWORK = 1,
	AWAKENED = 2
};

#endif