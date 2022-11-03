﻿#pragma once

#include "object-enchant/tr-flags.h"
#include "object-enchant/trg-types.h"
#include "system/angband.h"
#include "util/flag-group.h"
#include <string>
#include <vector>

enum class ItemKindType : short;
enum class RandomArtActType : short;
class BaseItemInfo {
public:
    BaseItemInfo() = default;
    KIND_OBJECT_IDX idx{};

    std::string name; /*!< ベースアイテム名 */
    std::string text; /*!< 解説テキスト */
    std::string flavor_name; /*!< 未確定名 */

    ItemKindType tval{}; /*!< ベースアイテム種別の大項目値 Object type */
    OBJECT_SUBTYPE_VALUE sval{}; /*!< ベースアイテム種別の小項目値 Object sub type */

    PARAMETER_VALUE pval{}; /*!< ベースアイテムのpval（能力修正共通値） Object extra info */

    HIT_PROB to_h{}; /*!< ベースアイテムの命中修正値 / Bonus to hit */
    int to_d{}; /*!< ベースアイテムのダメージ修正値 / Bonus to damage */
    ARMOUR_CLASS to_a{}; /*!< ベースアイテムのAC修正値 / Bonus to armor */
    ARMOUR_CLASS ac{}; /*!< ベースアイテムのAC基本値 /  Base armor */

    DICE_NUMBER dd{}; /*!< ダメージダイスの数 / Damage dice */
    DICE_SID ds{}; /*!< ダメージダイスの大きさ / Damage sides */

    WEIGHT weight{}; /*!< ベースアイテムの重量 / Weight */
    PRICE cost{}; /*!< ベースアイテムの基本価値 / Object "base cost" */
    TrFlags flags{}; /*!< ベースアイテムの基本特性ビット配列 / Flags */
    EnumClassFlagGroup<ItemGenerationTraitType> gen_flags; /*!< ベースアイテムの生成特性ビット配列 / flags for generate */

    DEPTH level{}; /*!< ベースアイテムの基本生成階 / Level */
    DEPTH locale[4]{}; /*!< ベースアイテムの生成階テーブル / Allocation level(s) */
    PROB chance[4]{}; /*!< ベースアイテムの生成確率テーブル / Allocation chance(s) */

    TERM_COLOR d_attr{}; /*!< デフォルトのアイテムシンボルカラー / Default object attribute */
    char d_char{}; /*!< デフォルトのアイテムシンボルアルファベット / Default object character */
    bool easy_know{}; /*!< ベースアイテムが初期からベース名を判断可能かどうか / This object is always known (if aware) */
    RandomArtActType act_idx{}; /*!< 発動能力のID /  Activative ability index */

    /* @todo ここから下はk_info.txt に依存しないミュータブルなフィールド群なので、将来的に分離予定 */

    TERM_COLOR x_attr{}; /*!< 設定変更後のアイテムシンボルカラー /  Desired object attribute */
    char x_char{}; /*!< 設定変更後のアイテムシンボルアルファベット /  Desired object character */

    IDX flavor{}; /*!< 未鑑定名の何番目を当てるか(0は未鑑定名なし) / Special object flavor (or zero) */
    bool aware{}; /*!< ベースアイテムが鑑定済かどうか /  The player is "aware" of the item's effects */
    bool tried{}; /*!< ベースアイテムを未鑑定のまま試したことがあるか /  The player has "tried" one of the items */
};

extern std::vector<BaseItemInfo> baseitems_info;
