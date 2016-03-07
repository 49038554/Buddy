/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50150
Source Host           : localhost:3306
Source Database       : gamebuddy

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-03-07 18:42:13
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `house`
-- ----------------------------
DROP TABLE IF EXISTS `house`;
CREATE TABLE `house` (
  `owner` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `houseId` int(11) NOT NULL DEFAULT '0',
  `name` varchar(255) DEFAULT NULL COMMENT '名字',
  `longitude` double DEFAULT NULL COMMENT '经度',
  `latitude` double DEFAULT NULL COMMENT '纬度',
  `address` varchar(255) DEFAULT NULL COMMENT '地址',
  `radius` int(11) DEFAULT NULL COMMENT '半径，最小100米',
  `coin` int(11) DEFAULT NULL COMMENT '花费虚拟币数量',
  PRIMARY KEY (`houseId`,`owner`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of house
-- ----------------------------
INSERT INTO `house` VALUES ('1', '1', '火羽1', '1.12121', '20.12121', '北京通州1', '100', '300000');
INSERT INTO `house` VALUES ('100', '1', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('1', '2', '火羽2', '2.12121', '20.12121', '北京通州2', '100', '300000');
INSERT INTO `house` VALUES ('100', '2', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('1', '3', '火羽3', '3.12121', '20.12121', '北京通州3', '100', '300000');
INSERT INTO `house` VALUES ('100', '3', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '4', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '5', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '6', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '7', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '8', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '9', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '10', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '11', 'house100', '100.123', '100.456', 'address', '100', '300000');
INSERT INTO `house` VALUES ('100', '12', 'house100', '100.123', '100.456', 'address', '100', '300000');

-- ----------------------------
-- Table structure for `pet`
-- ----------------------------
DROP TABLE IF EXISTS `pet`;
CREATE TABLE `pet` (
  `userId` int(11) NOT NULL COMMENT '玩家Id',
  `number` int(11) NOT NULL COMMENT '用户拥有的巴迪兽全国编号',
  `petId` int(11) NOT NULL COMMENT '宠物id',
  `talent` int(11) NOT NULL COMMENT '特性',
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
  PRIMARY KEY (`userId`,`number`,`petId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of pet
-- ----------------------------
INSERT INTO `pet` VALUES ('1', '56', '1', '1', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '98', '2', '115', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '99', '3', '30', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '102', '4', '46', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '104', '5', '44', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('1', '105', '6', '4', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('100', '98', '1', '10', '0', '31', '31', '31', '31', '31', '31', '131', '131', '131', '131', '131', '131');
INSERT INTO `pet` VALUES ('100', '98', '2', '115', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('100', '99', '3', '30', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('100', '102', '4', '46', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('100', '104', '5', '44', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');
INSERT INTO `pet` VALUES ('100', '105', '6', '4', '0', '25', '25', '25', '25', '25', '25', '0', '0', '0', '0', '0', '0');

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
INSERT INTO `pet_skill` VALUES ('100', '1', '1');
INSERT INTO `pet_skill` VALUES ('100', '1', '3');

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
INSERT INTO `player_item` VALUES ('100', '4', '1000');

-- ----------------------------
-- Table structure for `tree`
-- ----------------------------
DROP TABLE IF EXISTS `tree`;
CREATE TABLE `tree` (
  `owner` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `treeId` int(11) NOT NULL DEFAULT '0' COMMENT '果树id',
  `houseId` int(11) DEFAULT NULL COMMENT '据点id',
  `createTime` datetime DEFAULT NULL COMMENT '种下日期',
  PRIMARY KEY (`owner`,`treeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tree
-- ----------------------------
INSERT INTO `tree` VALUES ('1', '1', '1', '2016-03-04 14:46:21');
INSERT INTO `tree` VALUES ('1', '2', '2', '2016-03-04 14:46:34');
INSERT INTO `tree` VALUES ('1', '3', '3', '2016-03-04 14:46:36');
INSERT INTO `tree` VALUES ('100', '1', '1', '2016-03-07 17:07:15');
INSERT INTO `tree` VALUES ('100', '2', '2', '2016-03-07 17:09:55');
INSERT INTO `tree` VALUES ('100', '3', '3', '2016-03-07 17:11:45');
INSERT INTO `tree` VALUES ('100', '4', '4', '2016-03-07 17:13:22');
INSERT INTO `tree` VALUES ('100', '5', '5', '2016-03-07 17:19:12');
INSERT INTO `tree` VALUES ('100', '6', '6', '2016-03-07 17:20:31');
INSERT INTO `tree` VALUES ('100', '7', '7', '2016-03-07 17:34:31');
INSERT INTO `tree` VALUES ('100', '8', '8', '2016-03-07 17:38:18');
INSERT INTO `tree` VALUES ('100', '9', '9', '2016-03-07 17:39:41');
INSERT INTO `tree` VALUES ('100', '10', '10', '2016-03-07 17:41:12');
INSERT INTO `tree` VALUES ('100', '11', '11', '2016-03-07 18:28:04');
INSERT INTO `tree` VALUES ('100', '12', '12', '2016-03-07 18:37:03');
