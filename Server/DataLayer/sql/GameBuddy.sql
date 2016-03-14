/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50150
Source Host           : localhost:3306
Source Database       : gamebuddy

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-03-14 20:02:32
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for house
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
-- Table structure for pet
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
-- Table structure for pet_skill
-- ----------------------------
DROP TABLE IF EXISTS `pet_skill`;
CREATE TABLE `pet_skill` (
  `userId` int(11) NOT NULL DEFAULT '0' COMMENT '玩家Id',
  `petId` int(11) NOT NULL DEFAULT '0' COMMENT '宠物id',
  `race` int(11) NOT NULL DEFAULT '0' COMMENT '属性Id',
  PRIMARY KEY (`userId`,`petId`,`race`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for player
-- ----------------------------
DROP TABLE IF EXISTS `player`;
CREATE TABLE `player` (
  `userId` int(11) NOT NULL,
  `coin` int(255) DEFAULT NULL,
  `pet1` int(11) DEFAULT NULL,
  `pet2` int(11) DEFAULT NULL,
  `pet3` int(11) DEFAULT NULL,
  `pet4` int(11) DEFAULT NULL,
  `pet5` int(11) DEFAULT NULL,
  `pet6` int(11) DEFAULT NULL,
  PRIMARY KEY (`userId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for player_item
-- ----------------------------
DROP TABLE IF EXISTS `player_item`;
CREATE TABLE `player_item` (
  `userId` int(11) NOT NULL COMMENT '玩家Id',
  `itemId` int(11) NOT NULL COMMENT '物品Id',
  `count` int(11) NOT NULL COMMENT '拥有数量',
  PRIMARY KEY (`userId`,`itemId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for tree
-- ----------------------------
DROP TABLE IF EXISTS `tree`;
CREATE TABLE `tree` (
  `owner` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `treeId` int(11) NOT NULL DEFAULT '0' COMMENT '果树id',
  `houseId` int(11) DEFAULT NULL COMMENT '据点id',
  `createTime` datetime DEFAULT NULL COMMENT '种下日期',
  PRIMARY KEY (`owner`,`treeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
