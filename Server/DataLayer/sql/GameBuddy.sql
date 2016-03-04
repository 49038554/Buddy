/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50150
Source Host           : localhost:3306
Source Database       : gamebuddy

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-03-04 10:52:29
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `pet`
-- ----------------------------
DROP TABLE IF EXISTS `pet`;
CREATE TABLE `pet` (
  `userId` int(11) NOT NULL COMMENT '玩家Id',
  `number` int(11) NOT NULL COMMENT '用户拥有的巴迪兽全国编号',
  `petId` int(11) NOT NULL COMMENT '宠物id',
  `talent` int(11) NOT NULL COMMENT '特性',
  `skill1` int(11) NOT NULL,
  `skill2` int(11) NOT NULL,
  `skill3` int(11) NOT NULL,
  `skill4` int(11) NOT NULL,
  `nature` int(11) NOT NULL COMMENT '性格',
  `HPHealthy` int(11) NOT NULL COMMENT 'HP体质',
  `WGHealthy` int(11) NOT NULL COMMENT 'WG体质',
  `WFHealthy` int(11) NOT NULL COMMENT 'WF体质',
  `TGHealthy` int(11) NOT NULL COMMENT 'TG体质',
  `TFHealthy` int(11) NOT NULL COMMENT 'TF体质',
  `SDHealthy` int(11) NOT NULL COMMENT 'SD体质',
  `HPMuscle` int(11) DEFAULT '0' COMMENT 'HP努力',
  `WGMuscle` int(11) DEFAULT '0' COMMENT 'WG努力',
  `WFMuscle` int(11) DEFAULT '0' COMMENT 'WF努力',
  `TGMuscle` int(11) DEFAULT '0' COMMENT 'TG努力',
  `TFMuscle` int(11) DEFAULT '0' COMMENT 'TF努力',
  `SDMuscle` int(11) DEFAULT '0' COMMENT '速度努力',
  `itemId` int(11) DEFAULT '0' COMMENT '携带物品',
  PRIMARY KEY (`userId`,`number`,`petId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of pet
-- ----------------------------
INSERT INTO `pet` VALUES ('1', '56', '1', '1', '98', '26', '68', '70', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '98', '2', '76', '68', '74', '35', '531', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '99', '3', '30', '88', '16', '322', '340', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '102', '4', '46', '68', '14', '16', '357', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '104', '5', '44', '68', '74', '99', '520', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '105', '6', '4', '68', '57', '133', '264', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0', '0');

-- ----------------------------
-- Table structure for `pet_skill`
-- ----------------------------
DROP TABLE IF EXISTS `pet_skill`;
CREATE TABLE `pet_skill` (
  `userId` int(11) NOT NULL DEFAULT '0' COMMENT '玩家Id',
  `petId` int(11) NOT NULL DEFAULT '0' COMMENT '宠物id',
  `race` int(11) NOT NULL DEFAULT '0' COMMENT '属性Id',
  PRIMARY KEY (`userId`,`petId`,`race`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of pet_skill
-- ----------------------------
INSERT INTO `pet_skill` VALUES ('1', '1', '5');
INSERT INTO `pet_skill` VALUES ('1', '1', '9');

-- ----------------------------
-- Table structure for `player_item`
-- ----------------------------
DROP TABLE IF EXISTS `player_item`;
CREATE TABLE `player_item` (
  `userId` int(11) NOT NULL COMMENT '玩家Id',
  `itemId` int(11) NOT NULL COMMENT '物品Id',
  `count` int(11) NOT NULL COMMENT '拥有数量',
  PRIMARY KEY (`userId`,`itemId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of player_item
-- ----------------------------
INSERT INTO `player_item` VALUES ('1', '4', '1000');
INSERT INTO `player_item` VALUES ('1', '8', '1');
