/*
 Navicat Premium Data Transfer

 Source Server         : RDS
 Source Server Type    : MySQL
 Source Server Version : 50725
 Source Host           : rm-uf68ca242e253b08dio.mysql.rds.aliyuncs.com:3306
 Source Schema         : sourcemod

 Target Server Type    : MySQL
 Target Server Version : 50725
 File Encoding         : 65001

 Date: 15/03/2022 14:59:08
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for zemaps
-- ----------------------------
DROP TABLE IF EXISTS `zemaps`;
CREATE TABLE `zemaps`  (
  `ID` int(11) NOT NULL,
  `NAME` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `COOLDOWN` int(11) NULL DEFAULT NULL,
  `CN_NAME` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `LAST_RUN_TIME` int(11) NULL DEFAULT NULL,
  `AVAILABLE` int(11) NULL DEFAULT NULL,
  `COST` int(11) NULL DEFAULT NULL,
  `HEAT` int(11) NULL DEFAULT NULL,
  `DIFFICULTY` int(11) NULL DEFAULT NULL,
  `TAG` int(11) NULL DEFAULT NULL,
  `ROUND` int(11) NULL DEFAULT NULL,
  `WINS` int(11) NULL DEFAULT NULL,
  `TRANSLATED` int(11) NULL DEFAULT NULL,
  `DOWNLOAD` int(11) NULL DEFAULT NULL,
  `EXIST` int(11) NULL DEFAULT NULL,
  `ROUNDTIME` int(11) NULL DEFAULT NULL,
  `TIMELIMIT` int(11) NULL DEFAULT NULL,
  `INFECTTIME` int(11) NULL DEFAULT NULL,
  `RANDOM` int(11) NULL DEFAULT NULL,
  `EXTEND` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of zemaps
-- ----------------------------
INSERT INTO `zemaps` VALUES (1, 'ze_2012_p3', 720, '2012', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (2, 'ze_30_seconds__b21_p2', 720, '30秒真男人', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (3, 'ze_8bit_csgo3', 720, '8bit', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (4, 'ze_abandoned_industry_va2', 720, '废弃工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (5, 'ze_abandoned_project_v1_2_csgo', 720, '废弃计划', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (6, 'ze_alchemy_b7', 720, '神秘力量', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (7, 'ze_alien_mountain_escape_p3_fix', 720, '阿里山', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (8, 'ze_alien_shooter_gp1_3', 720, '异星枪手', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (9, 'ze_alien_vs_predator_v6go_fix', 720, '异形大战铁血战士', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (10, 'ze_amalgamation_v1_1', 720, '融合', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (11, 'ze_ancient_wrath_v2_test27_g6t', 720, '古代种', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (12, 'ze_aooka_v3_t5', 720, '龙之殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (13, 'ze_aot_trost_v6', 720, '进击的巨人:特罗斯特', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (14, 'ze_aperturescience_p2', 720, '光圈科技', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (15, 'ze_apocatastasis_v1_2', 720, '万恶消散', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (16, 'ze_apollo_p2', 720, '阿波罗', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (17, 'ze_aragami_v1_1', 720, '荒神', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (18, 'ze_arcade_parade_b6', 720, '小游戏：巡回', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (19, 'ze_arctic_escape_p2', 720, '北极逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (20, 'ze_artika_base_v1_2', 720, '北极基地', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (21, 'ze_ashen_keep_p', 720, '黑暗之魂：亚诺尔隆德', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (22, 'ze_atix_apocalypse_p3', 720, 'ATIX：天启', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (23, 'ze_atix_panic_b3t_p', 720, 'ATIX：恐慌_b3t', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (24, 'ze_atix_panic_v1', 720, 'ATIX：恐慌_v1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (25, 'ze_atos_v2_3', 720, '阿托斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (26, 'ze_azathoth_v1_1', 720, '阿撒托斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (27, 'ze_aztecnoob_p2', 720, '阿兹特克：noob', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (28, 'ze_aztec_temple_p', 720, '阿兹特克：神庙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (29, 'ze_a_e_s_t_h_e_t_i_c_v1_1', 720, '舞台剧', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (30, 'ze_backrooms_p3', 720, '密室逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (31, 'ze_barrage_p2', 720, '大坝逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (32, 'ze_bathroom_v2_5', 720, '巨人浴室', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (33, 'ze_bigfoot_escapeica_v3_4', 720, '大脚怪', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (34, 'ze_biohazard2_rpd_004_p3', 720, '生化危机2：浣熊市警察局', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (35, 'ze_biohazard2_sewer_j2_004', 720, '生化危机2：下水道', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (36, 'ze_biohazard_manor_004_p4', 720, '生化危机：洋馆', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (37, 'ze_bioshock_v6_2_csgo7', 720, '生化奇兵', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (38, 'ze_bisounours_party_p2', 720, '彩虹岛', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (39, 'ze_blackmesa_remix_p2', 720, '黑山基地', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (40, 'ze_blue_magic_castle_v3_1', 720, '青魔法城堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (41, 'ze_blue_zenith_v0_2', 720, '苍蓝天顶', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (42, 'ze_boacceho_p4', 720, '古堡逃脱', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (43, 'ze_boatescape101_p', 720, '坐船跑101', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (44, 'ze_boatescape111_v1fix', 720, '坐船跑111', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (45, 'ze_boatescape999_v5', 720, '坐船跑999', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (46, 'ze_boatescapex_b8', 720, '坐船炮：加强版b8', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (47, 'ze_boatescape_extendido_p4', 720, '坐船跑：究极无敌黄金加强版', 1647237280, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (48, 'ze_boredom_p', 720, '无聊的图', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (49, 'ze_bp-infested-prison_p2', 720, '监狱逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (50, 'ze_breakable_p3', 720, '大破坏', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (51, 'ze_breezy_v4', 720, '轻松的图', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (52, 'ze_bulhufas_a1_1', 720, '布卢法斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (53, 'ze_castlevania_64_v1_3_b9_go', 720, '恶魔城v1_3', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (54, 'ze_castlevania_p1_7', 720, '恶魔城p1_7', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (55, 'ze_castle_bridge_v1_4', 720, '大桥堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (56, 'ze_castle_of_the_bladekeeper_t1', 720, '执剑人城堡', 1647236126, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (57, 'ze_chaos_v7_7', 720, '混沌', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (58, 'ze_check_b1_1f', 720, '西洋棋', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (59, 'ze_chicken_lords_tower_ig1', 720, '小鸡城堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (60, 'ze_chicken_ranch_v2_2', 720, '小鸡农场', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (61, 'ze_christmas_infection_p2', 720, '圣诞逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (62, 'ze_coastalcity_b9', 720, '海滨之城', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (63, 'ze_colorlicouspilar_p4', 720, '彩虹塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (64, 'ze_colors_p3', 720, '搞颜色', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (65, 'ze_complex_va3', 720, '建筑群', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (66, 'ze_contradiction_spiral_a2_1', 720, '矛盾螺旋', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (67, 'ze_core_v2b', 720, '核心', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (68, 'ze_crashbandicoot_p3', 720, '古惑狼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (69, 'ze_crazykart_v4', 720, '疯狂卡丁车', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (70, 'ze_crypt_v1_4', 720, '墓穴', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (71, 'ze_cursed_bear_tales_v1_2', 720, '诅咒熊之传说', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (72, 'ze_cursed_ruins_gf1_1', 720, '咒怨遗迹', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (73, 'ze_cyberderp_p3_kia', 720, '赛博魔幻逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (74, 'ze_cyberia_v1', 720, '赛博利亚', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (75, 'ze_cyberpunk_x_a07', 720, '赛博之城', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (76, 'ze_dangerous_waters_v2_final', 720, '危险水域', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (77, 'ze_dark_souls_ptd_csgo2', 720, '黑暗之魂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (78, 'ze_deadcore_s7f2', 720, '死亡核心（s7f2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (79, 'ze_deadcore_v1_3', 720, '死亡核心（v1.3）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (80, 'ze_dear_v1', 720, '月神宫', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (81, 'ze_deathinvain_palace_v1_9', 720, '枉死宫殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (82, 'ze_death_star_escape_v4_3_p2', 720, '死星逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (83, 'ze_death_wish_gf1_1', 720, '死亡之愿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (84, 'ze_deepice_p3', 720, '深井冰', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (85, 'ze_defense3002', 720, '守点3002', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (86, 'ze_descent_into_cerberon_v1', 720, '破碎行动', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (87, 'ze_desert_fox_v7_6f', 720, '沙漠之狐', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (88, 'ze_desperate_soul_a9', 720, '绝望灵魂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (89, 'ze_destruction_of_exorath_v4_lite', 720, '黑影空间：毁灭', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (90, 'ze_djinn_go', 720, '神灵：阿拉伯神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (91, 'ze_doom_p2', 720, '毁灭战士', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (92, 'ze_draco_accidere_v4_final', 720, '龙之坠落', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (93, 'ze_dreamin_v3_3', 720, '梦想', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (94, 'ze_dungeon_of_ignorance_a1_2', 720, '迷障地牢', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (95, 'ze_dust_escape_p3', 720, '沙漠逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (96, 'ze_easy_escape_r', 720, 'EZ逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (97, 'ze_eden_a3', 720, '伊甸', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (98, 'ze_einstein_v7', 720, '爱因斯坦', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (99, 'ze_elevator_escape_p3', 720, '新电梯逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (100, 'ze_epic_escape_v2_1', 720, '史诗逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (101, 'ze_escapetheworld_a33', 720, '逃离世界', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (102, 'ze_escape_horizon_p3', 720, '逃离地平线', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (103, 'ze_escape_stroggos_p2', 720, '逃离史卓格斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (104, 'ze_escape_the_eye_p3', 720, '逃离法眼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (105, 'ze_eternal_grove_v3', 720, '永恒果林', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (106, 'ze_eternal_journey_go_v1_4t11', 720, '飞升者', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (107, 'ze_eternal_void_p3', 720, '永恒虚空', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (108, 'ze_evernight_a3_4', 720, '永夜神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (109, 'ze_evil_mansion_v1_finalf', 720, '整蛊豪宅', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (110, 'ze_exchange_innovation_v1b', 720, '逃离生化大楼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (111, 'ze_fapescape_p5', 720, '乐高大逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (112, 'ze_fapescape_rote_v1_3', 720, '乐高_v1_3', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (113, 'ze_fast_escape_p2', 720, '快速逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (114, 'ze_ffuxx_demon_naramina_a2', 720, '最终幻想114：纳拉米娜雪域城堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (115, 'ze_ffvii_cosmo_canyon_v5k_fix', 720, '最终幻想7：宇宙峡谷（v5）', 1647234146, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (116, 'ze_ffvii_cosmo_canyon_v6test4', 720, '最终幻想7：宇宙峡谷（v6 t4）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (117, 'ze_ffvii_mako_reactor_v5_3_v5', 60, '最终幻想7：魔光炉(v5_3)', 1647236592, 1, 0, 0, 0, 0, 67, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (118, 'ze_ffvii_mako_reactor_v6_b09k2', 200, '最终幻想7：魔光炉（v6 b09）', 1645508867, 1, 50, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (119, 'ze_ffvii_snow_cosmo_v5_5f', 720, '最终幻想7：宇宙峡谷（雪v5）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (120, 'ze_ffvii_temple_ancient_v4_3', 720, '最终幻想7：古代种神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (121, 'ze_ffxii_feywood_b6_1k', 720, '最终幻想12：幻妖森林b6_1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (122, 'ze_ffxii_feywood_b6_3k', 720, '最终幻想12：幻妖森林b6_3', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (123, 'ze_ffxii_mt_bur_omisace_v5_4fix4', 720, '最终幻想12:神都奥尔米赛斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (124, 'ze_ffxii_paramina_rift_v1_4', 720, '最终幻想12：帕拉米亚大峡谷', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (125, 'ze_ffxii_paramina_rift_v1_4_a8t', 720, '最终幻想12：帕拉米亚大峡谷（a8t）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (126, 'ze_ffxii_ridorana_cataract_t5_3_w6', 720, '最终幻想12：里多拉纳大瀑布', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (127, 'ze_ffxii_salikawood_v1_6f', 720, '最终幻想12：萨利卡树林', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (128, 'ze_ffxii_westersand_v7_z9', 720, '最终幻想12：西达尔玛斯卡沙漠（v7z9）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (129, 'ze_ffxii_westersand_v8zeta1k', 720, '最终幻想12：西达尔玛斯卡沙漠（v8z1）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (130, 'ze_ffxii_westersand_v8zeta1_x18', 720, '最终幻想12：西达尔玛斯卡沙漠（x18）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (131, 'ze_ffxiv_pharos_sirius_v1_6', 720, '最终幻想14:领航明灯天狼星灯塔v1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (132, 'ze_ffxiv_wanderers_palace_v2_va2', 720, '最终幻想14：放浪神古神殿（v2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (133, 'ze_ffxiv_wanderers_palace_v4_va2', 720, '最终幻想14：放浪神古神殿（v4）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (134, 'ze_ffxiv_wanderers_palace_v5_2', 720, '最终幻想14：放浪神古神殿（v5.2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (135, 'ze_ffxiv_wanderers_palace_v6', 720, '最终幻想14：放浪神古神殿（v6）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (136, 'ze_firewall_laboratory_part1_p2', 720, '防火墙1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (137, 'ze_firewall_laboratory_part2_p2', 720, '防火墙2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (138, 'ze_flex_b4', 720, '方块闯关', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (139, 'ze_flying_world_p', 720, '飞翔世界', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (140, 'ze_forbidden_repository_a1_4', 720, '禁地仓库', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (141, 'ze_forest_r_v1', 720, '森林', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (142, 'ze_forgotten_facility_p3', 720, '遗忘工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (143, 'ze_forhidden_slicoo_v1', 720, '禁地斯狄科', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (144, 'ze_forius_just_run_p2', 720, '只管跑', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (145, 'ze_forsaken_temple_v3_b5', 720, '荒废神庙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (146, 'ze_frostdrake_tower_v1', 720, '冰龙塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (147, 'ze_frozentemple_rc1_1', 720, '冰冻神庙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (148, 'ze_frozen_abyss_v1_2', 720, '寒冰深渊', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (149, 'ze_funny_runner_v3_1', 720, '娱乐跑跑', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (150, 'ze_games_v2_1', 720, '小游戏', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (151, 'ze_genso_of_last_v3_4_t1', 720, '永远的幻想', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (152, 'ze_gods_wrath_p2', 720, '神怒', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (153, 'ze_got_the_north_csgo3', 720, '权力的游戏：北境', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (154, 'ze_grand_boatescape_p3', 720, '坐船跑：终极加强版', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (155, 'ze_grau_a03_4ff', 720, '格劳', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (156, 'ze_greece_escape_p', 720, '希腊逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (157, 'ze_greencity_fix', 720, '绿之城', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (158, 'ze_gris_a4_9_8', 720, '灰（a498）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (159, 'ze_gris_v1_13', 720, '灰（v1.13）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (160, 'ze_halloween_house_b4', 720, '万圣节小屋', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (161, 'ze_halo_p3', 720, '光环', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (162, 'ze_halo_poa_p4', 720, '光环：秋风之墩号', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (163, 'ze_harry_potter_v2_1_csgo', 720, '哈利波特', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (164, 'ze_haunted_lab_escape_p3', 720, '闹鬼实验室', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (165, 'ze_hazard_escape_p2', 720, '冒死逃生', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (166, 'ze_hell_escape_p2', 720, '地狱逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (167, 'ze_hidden_fortress_p', 720, '隐秘要塞', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (168, 'ze_hold_em_v8', 720, '鬼脸长廊', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (169, 'ze_honkai_impact_3rd_babel_v1_55', 720, '崩坏三:巴比伦塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (170, 'ze_hsc_a4_q3', 720, '维度超越', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (171, 'ze_hypernova_p2', 720, '超新星', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (172, 'ze_iamlegend_p3', 720, '我是传奇', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (173, 'ze_icecap_escape_v5_p', 720, '西瓜橘子坐船跑', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (174, 'ze_iceland_escape_v1_2', 720, '冰岛逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (175, 'ze_illya_b4_csgo_a3', 720, '伊利亚', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (176, 'ze_inboxed_final_fix', 720, '盒子图', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (177, 'ze_indiana_jones_v4a_004_p4', 720, '夺宝奇兵', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (178, 'ze_industrial_dejavu_g2t', 720, '工业循环', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (179, 'ze_infected_lab_v4_1', 720, '生化实验室', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (180, 'ze_infected_sewers_csgo', 720, '生化下水道', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (181, 'ze_infected_tramway_csgo1', 720, '生化电车', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (182, 'ze_infested-industry_p2', 720, '生化工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (183, 'ze_infiltration_final_r1', 720, '渗透', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (184, 'ze_insanitycity_va2', 720, '不可名状之城', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (185, 'ze_interception_p2', 720, '阻截', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (186, 'ze_island_escape_p3', 720, '岛屿逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (187, 'ze_isla_nublar_p3', 720, '侏罗纪公园：恐龙岛', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (188, 'ze_italy_town_p2', 720, '意大利小镇', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (189, 'ze_jamais_vu_b2', 720, '未视感', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (190, 'ze_jiangnan_town_v3', 720, '江南', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (191, 'ze_jiba_journey_v4_3', 720, 'JoJo的奇妙冒险（v4.3）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (192, 'ze_jjba_v7', 720, 'JoJo的奇妙冒险', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (193, 'ze_johnny_nukem_go_b8_3a', 720, '毁灭强尼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (194, 'ze_journey_p2', 720, '黑暗之魂:旅途', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (195, 'ze_jurassicpark_escape_p2', 720, '侏罗纪公园p2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (196, 'ze_jurassicpark_p3', 720, '侏罗纪公园p3', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (197, 'ze_k19_escape_go1', 720, 'K19机库', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (198, 'ze_kaffe_escape_v2', 720, '咖啡逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (199, 'ze_kage_a3', 720, '赤影战士', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (200, 'ze_kitchen_v2', 720, '厨房逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (201, 'ze_knife_stray_v3', 720, '秒杀刀逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (202, 'ze_kororinpa_v2_2', 720, '转转球迷宫', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (203, 'ze_kraznov_poopata_p2', 720, '克拉兹诺夫', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (204, 'ze_l0v0l_v1_4_csgo2', 720, 'l0v0l', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (205, 'ze_last_man_standing_v6_t2', 720, '一夫当关', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (206, 'ze_legoland_crackheads_p2', 720, '乐高：哈草行动', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (207, 'ze_lemonysnickets_p2', 720, '雷蒙·斯尼奇历险', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (208, 'ze_licciana_escape_p', 720, '里西西亚那', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (209, 'ze_lila_panic_escape_tr2', 720, '里拉大逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (210, 'ze_lostclue_b3_3', 720, '异乱之战：追击', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (211, 'ze_lotr_helms_deep_v5_p6', 720, '魔戒：圣盔谷', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (212, 'ze_lotr_isengard_csgo1', 720, '魔戒：艾辛格', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (213, 'ze_lotr_laketown_v3_3', 720, '魔戒：长湖镇', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (214, 'ze_lotr_minas_tiret_p2', 720, '魔戒：小米纳斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (215, 'ze_lotr_minas_tirith_p5', 720, '魔戒：米那斯提力斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (216, 'ze_lotr_mines_of_moria_p2', 720, '魔戒：摩瑞亚矿坑', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (217, 'ze_lotr_mirkwood_v2_5', 720, '魔戒：黑森林', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (218, 'ze_lotr_mount_doom_p3', 720, '魔戒：末日山', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (219, 'ze_luciddreams_f5_6', 720, '清醒梦', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (220, 'ze_luck_matters_v3_1', 720, '幸运逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (221, 'ze_lucy_escape_v1', 720, '樱花神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (222, 'ze_luigismansion_p3', 720, '路易基鬼屋', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (223, 'ze_lux_umbra_v1', 720, '光与影前传', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (224, 'ze_m0w0m_csgo1', 720, 'm0w0m', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (225, 'ze_mabinogi_f4_1', 720, '洛奇', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (226, 'ze_madara_temple_v1fix_csgo', 720, '宇智波斑神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (227, 'ze_magala_castle_final', 720, '马格拉城堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (228, 'ze_magic_carpet_p2', 720, '魔法飞毯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (229, 'ze_mapeadores_museum_v1', 720, 'MAPPER的地图博物馆', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (230, 'ze_mario_tower_v4_p2', 720, '马里奥之塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (231, 'ze_mars_escape_gp3', 720, '火星逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (232, 'ze_meatropolis_p2', 720, '血肉之都', 1647231353, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (233, 'ze_medievil_the_graveyard_v4', 720, '中世纪：坟场', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (234, 'ze_melong_dow_learjet_gp1', 720, '喷气机逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (235, 'ze_mgden_v2_5fix', 720, 'mgden方块', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (236, 'ze_military_escape_p4', 720, '军事基地逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (237, 'ze_mindtree_v1_6', 720, '智慧树', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (238, 'ze_minecraft_adventure_v1_3d', 720, '我的世界：大冒险', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (239, 'ze_minecraft_universe_v1_5', 720, '我的世界：宇宙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (240, 'ze_minecraft_v1_1_p3', 720, '我的世界：小mc', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (241, 'ze_minimal_csgo', 720, '最小值', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (242, 'ze_mirrors_edge_reborn_p3', 720, '镜之边缘', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (243, 'ze_mist_q2', 720, '雾龙传说（q2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (244, 'ze_mist_q3', 720, '雾龙传说', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (245, 'ze_mojos_minigames_v1_4_1', 720, '迷你游戏', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (246, 'ze_moon_base_p3', 720, '月球基地', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (247, 'ze_motanuminc_p2', 720, '光圈：后日谈', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (248, 'ze_mytex_escape_v3_p', 720, 'mytex逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (249, 'ze_naiads_v4a_6f', 720, '水泽仙女', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (250, 'ze_nameless_necropolis_v1_4', 720, '无名死城', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (251, 'ze_namviet_gf1_1', 720, '南越', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (252, 'ze_naruto_p', 720, '火影忍者', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (253, 'ze_natural_v1_4', 720, '自然', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (254, 'ze_necromanteion_v3_1', 720, '死灵神殿：希腊', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (255, 'ze_negative_legacy_va2', 720, '拒绝', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (256, 'ze_neko_canyon_p3', 720, '猫之谷', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (257, 'ze_nightmare_dimension_p3', 720, '梦魇幻境', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (258, 'ze_noir_p', 720, '黑色街道', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (259, 'ze_nomada_h3', 720, '无限宫院', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (260, 'ze_nora_v1_11', 720, '诺拉', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (261, 'ze_nostromo_escape_p2', 720, '诺斯特罗莫', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (262, 'ze_nostromo_redux_v1_2', 720, '新诺斯特罗莫', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (263, 'ze_notredame_p4', 720, '巴黎圣母院', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (264, 'ze_no_name_v1_6', 720, '无名', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (265, 'ze_no_title_v2_5', 720, '无题', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (266, 'ze_nuke_p3', 720, '核子危机：逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (267, 'ze_ny_marathon_p2', 720, '马拉松', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (268, 'ze_obj_npst_v1_2', 720, '行动：NPST（v1.2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (269, 'ze_obj_npst_v2', 720, '行动：NPST（v2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (270, 'ze_obj_rampage_v1_2', 720, '行动代号：狂怒', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (271, 'ze_Obj_Void_b3_t', 720, '行动代号：虚空（至死不渝）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (272, 'ze_oblivion_castle_v6', 720, '遗忘城堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (273, 'ze_ocean_base_escape_p2', 720, '海底基地', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (274, 'ze_offliner_v2_csgo1', 720, '单机者', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (275, 'ze_omega_p3', 720, '欧米茄', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (276, 'ze_omen_090', 720, '预兆', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (277, 'ze_omochix_v3_2fix', 720, '元素秘神', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (278, 'ze_omochi_b10', 720, '元素秘神b10', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (279, 'ze_oneiros_p', 720, '奥内罗斯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (280, 'ze_oot_shadowtemple_b10', 720, '塞尔达传说：时之笛', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (281, 'ze_opti_variety_missions_p3', 720, '多样任务', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (282, 'ze_otakuroom_v5_6f3', 720, '肥宅房', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (283, 'ze_outpost_escape_k2', 720, '哨站逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (284, 'ze_pallaroid_rage_v1', 720, '帕拉洛伊德之怒', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (285, 'ze_pandora_revolutionz_hdr2', 720, '潘多拉', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (286, 'ze_paper_escaper_p7', 720, '纸质逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (287, 'ze_paranoid_rezurrection_csgo1', 720, '游乐园:csgo1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (288, 'ze_parkers_pit_p', 720, '帕克矿坑', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (289, 'ze_parking_p4', 720, '停车场', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (290, 'ze_pc_v1_2', 720, '机箱逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (291, 'ze_pd2_crime_art_v2', 720, '收获日2：罪恶美术馆', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (292, 'ze_phantasy_escape_p3', 720, '幻想逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (293, 'ze_pirates_port_royal_v5_6', 720, '加勒比海盗：皇家军港', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (294, 'ze_pitchblack_p2', 720, '黑之音律', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (295, 'ze_pkmn_adventure_v9_3', 720, '宝可梦大冒险', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (296, 'ze_platformer_b6', 720, '捡币', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (297, 'ze_pokemon_rivals_v1_8', 720, '宝可梦竞技场', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (298, 'ze_portal_story_p2', 720, '传送门', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (299, 'ze_por_island_p3', 720, '刷分岛', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (300, 'ze_postal_v1_1', 720, '夺命邮差', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (301, 'ze_potc_iv_p3', 720, '加勒比海盗4', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (302, 'ze_potc_p4', 720, '加勒比海盗', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (303, 'ze_predator_ultimate_p7', 720, '铁血战士', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (304, 'ze_prototype_p2', 720, '虐杀原形', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (305, 'ze_puta_p3', 720, '工厂逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (306, 'ze_rabanastre_royal_t5', 720, '王城拉巴纳斯塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (307, 'ze_raccoon_facility_v6b_p', 720, '生化危机：蜂巢', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (308, 'ze_rainbow_factory_v2', 720, '彩虹工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (309, 'ze_ramp_a1_1', 720, '斜坡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (310, 'ze_randomizer_c1', 720, '新随机小游戏', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (311, 'ze_random_p2', 720, '随机小游戏', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (312, 'ze_ratescape_v5_4', 720, '鼠鼠逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (313, 'ze_reconnoiter_b5', 720, '侦察行动', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (314, 'ze_retribution_pf', 720, '美国大兵影视基地', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (315, 'ze_rev_a6_3', 720, 'rev', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (316, 'ze_rizomata_va2', 720, '精灵塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (317, 'ze_rockescape_go2_1', 720, '岩石逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (318, 'ze_rollermine_factory_2010_p', 720, '滚雷工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (319, 'ze_rooftop_runaway1_p', 720, '屋顶逃亡1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (320, 'ze_rooftop_runaway2_p', 720, '屋顶逃亡2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (321, 'ze_roof_adventure_v8f', 720, '屋顶历险', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (322, 'ze_roscente_go1', 720, '罗森特', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (323, 'ze_rtcw_ominous_rumors_p4', 720, '重返德军总部', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (324, 'ze_rtv_one_more', 720, '\"就等最后这一票rtv了\"', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (325, 'ze_rush_b_v1_4', 720, 'Rush B', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (326, 'ze_r_w_b_ulti', 720, 'rwb', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (327, 'ze_sakuzyo_b2', 720, '削除', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (328, 'ze_sandstorm_go_v1_5', 720, '沙尘暴', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (329, 'ze_santassination_v3', 720, '狮子王：圣诞老人之罪', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (330, 'ze_saw_p3', 720, '电锯惊魂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (331, 'ze_schizophrenic_c3', 720, '精神分裂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (332, 'ze_sc_turtles_v5_p2', 720, '王八船', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (333, 'ze_serious_sam_tfe_v1_4', 720, '英雄萨姆', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (334, 'ze_serpentis_temple_csgo1', 720, '美杜莎神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (335, 'ze_serpentis_temple_p2', 720, '美杜莎神殿（p2）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (336, 'ze_sewage_processing_v8', 720, '污水处理厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (337, 'ze_sewer_escape_p3', 720, '下水道逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (338, 'ze_sg1_missions_v2_1_p2', 720, '星门行动', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (339, 'ze_shaun_of_the_dead_p', 720, '僵尸肖恩', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (340, 'ze_showdown_v4_1_p', 720, '对决', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (341, 'ze_shrine_of_umos_b3', 720, '乌莫斯神庙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (342, 'ze_shroomforest2_p6', 720, '蘑菇森林2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (343, 'ze_shroomforest3_p', 720, '蘑菇森林3', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (344, 'ze_shroomforest_p6', 720, '蘑菇森林1', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (345, 'ze_silent_hill_2_illusion_b5', 720, '寂静岭2：幻境', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (346, 'ze_silent_hill_3_dawn_v2', 720, '寂静岭3：黎明', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (347, 'ze_silent_hill_blackblood_f6', 720, '寂静岭：黑血', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (348, 'ze_skill_escape_v09', 720, '技巧逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (349, 'ze_skygarden_v2_1', 720, '天空花园', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (350, 'ze_skyscraper_city_r2', 720, '摩天大楼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (351, 'ze_slender_escape_rc2', 720, '面条人', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (352, 'ze_snowescape_gp1', 720, '雪域逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (353, 'ze_sorrento_escape_v2', 720, '索伦托', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (354, 'ze_space_flight_p3', 720, '太空航行', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (355, 'ze_sst2_p', 720, '星河舰队2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (356, 'ze_sst_b2_1_p5', 720, '星河舰队', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (357, 'ze_stalkermonolit_v8gofix1', 720, '潜行者：黑石特异区', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (358, 'ze_stalker_ultimate_v2_3_gg', 720, '潜行者：切尔诺贝利v2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (359, 'ze_stalker_ultimate_v4_9', 720, '潜行者：切尔诺贝利v4', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (360, 'ze_stargate_escape_v8_p2', 720, '星门逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (361, 'ze_star_wars_prequel_f1', 720, '星球大战：前传', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (362, 'ze_stilshrine_of_miriam_v6_2', 720, '米利安神庙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (363, 'ze_strange_escape_p2', 720, '奇异逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (364, 'ze_sumai_facility_v1a', 720, '烧麦工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (365, 'ze_sunkentemple_v3_1', 720, '魔兽世界:沉没神庙', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (366, 'ze_sunlight_p2', 720, '日光', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (367, 'ze_surf_danger_p2', 720, '滑翔：危境', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (368, 'ze_surf_dark_fantasy_v2go2', 720, '滑翔：黑暗幻想', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (369, 'ze_surf_facility_p4', 720, '滑翔：工厂', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (370, 'ze_surf_gypt_v1_3_1f', 720, '滑翔：埃及', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (371, 'ze_surf_sahok_p3', 720, '滑翔：萨霍克', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (372, 'ze_surf_shonyudo_v5_2', 720, '滑翔：山涧', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (373, 'ze_surf_vortex_v2_6', 720, '滑翔：涡旋', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (374, 'ze_survivors_p2', 720, '幸存者', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (375, 'ze_s_a_m_v1_7', 720, '剑与魔法', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (376, 'ze_taboo_carnation_p2', 720, '博塔尼亚半岛行动', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (377, 'ze_temple_escape_va3', 720, '神庙逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (378, 'ze_tenkinoko_welkin_v1_6f', 720, '天气之子', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (379, 'ze_tesv_skyrim_v5_6', 720, '上古卷轴5：天际', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (380, 'ze_thelostworld_redux_p2', 720, '失落世界', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (381, 'ze_the_curse_of_blackwater_v6b', 720, '黑水镇之诅咒', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (382, 'ze_the_end_v3f', 720, '末路穷途', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (383, 'ze_thousand_steps_gf1_2', 720, '千层阶梯', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (384, 'ze_three_eyes_v1_1', 720, '三眼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (385, 'ze_thriller_escape_p', 720, '惊悚逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (386, 'ze_tilex_ultimate_v2_13e', 720, '跳刀列车', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (387, 'ze_timesink_v1_2', 720, '时间之沉沦', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (388, 'ze_timesplitters_p2', 720, '时空撕裂者', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (389, 'ze_titanic_escape_p2', 720, '泰坦尼克', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (390, 'ze_tkara_v4_3', 720, '塔卡拉', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (391, 'ze_toaster_p3', 720, '烤面包机', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (392, 'ze_totemo_roka_v1', 720, '神秘海域', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (393, 'ze_touhou_gensokyo_o4', 720, '东方幻想乡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (394, 'ze_traak_b2', 720, '逃命游轮', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (395, 'ze_trainescape_final_p2', 720, '列车逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (396, 'ze_trainstation_tunnels_p', 720, '列车站：隧道', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (397, 'ze_trials_v3_test', 720, '审判', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (398, 'ze_tropico_escape_gf1_1', 720, '热带逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (399, 'ze_tv_station_r4', 720, '电视台', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (400, 'ze_tyranny2_v1_csgo2', 720, '暴君2', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (401, 'ze_tyranny_v5_2k3', 720, '暴君', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (402, 'ze_Uchiha_Legacy_cm2_fix', 720, '宇智波传奇', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (403, 'ze_underground_escape_p', 720, '地底逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (404, 'ze_undersea_temple_a4_4', 720, '海底神殿', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (405, 'ze_undertale_g_v1_3_fix1', 720, '传说之下', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (406, 'ze_urbanterror_b2', 720, '焦土策略', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (407, 'ze_urban_laby_of_death_p2', 720, '城市：死亡迷宫', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (408, 'ze_ut2004_convoy_r5', 720, '虚幻竞技场\'04', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (409, 'ze_Uyuni_v1_6', 720, '乌尤尼', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (410, 'ze_v0u0v_a6_csgo1', 720, 'v0u0v', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (411, 'ze_valkvang_v1_2', 720, '瓦尔克狱堡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (412, 'ze_venice_escape_p3', 720, '威尼斯逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (413, 'ze_vietnam_p3', 720, '越南', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (414, 'ze_visualizer_v1_2', 720, '监视者', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (415, 'ze_volcano_escape_p2', 720, '火山逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (416, 'ze_voodoo_islands_p5', 720, '逃离巫毒岛', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (417, 'ze_watagi_tunnel_v1_1', 720, '渡木隧道', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (418, 'ze_worlds_v1_2', 720, '世界', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (419, 'ze_world_escapes_v1', 720, '世界逃亡', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (420, 'ze_yueyingchuanshuo_10_8_f4', 720, '月影传说', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (421, 'ze_zk_mall_of_zombies_p2', 720, '购物中心', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (422, 'ze_zombiemod_corporation_p2', 720, '扛水塔', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (423, 'ze_zombierig_p3', 720, '钻井平台', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (424, 'zm_dust_itembattle_ultimate_f3', 720, '激 情 沙 城 在 线 乱 斗（f3）', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (425, 'zm_dust_itembattle_ultimate_f4', 720, '激 情 沙 城 在 线 乱 斗', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (426, 'zm_lostcities_p', 720, '生化金字塔：纯净版', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 60, 30, 15, 1, 3);
INSERT INTO `zemaps` VALUES (427, 'zm_resort_e', 720, '生化酒店_脚本版', 1645508867, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 60, 30, 15, 1, 3);

SET FOREIGN_KEY_CHECKS = 1;
