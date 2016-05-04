/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50150
Source Host           : localhost:3306
Source Database       : cluster

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-05-04 22:36:14
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for net_line
-- ----------------------------
DROP TABLE IF EXISTS `net_line`;
CREATE TABLE `net_line` (
  `id` int(11) NOT NULL COMMENT '网络线路id',
  `name` varchar(255) DEFAULT NULL COMMENT '运营商名',
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of net_line
-- ----------------------------
INSERT INTO `net_line` VALUES ('0', '未知');
INSERT INTO `net_line` VALUES ('1', '电信');
INSERT INTO `net_line` VALUES ('2', '移动');
INSERT INTO `net_line` VALUES ('3', '联通');

-- ----------------------------
-- Table structure for node
-- ----------------------------
DROP TABLE IF EXISTS `node`;
CREATE TABLE `node` (
  `name` varchar(255) NOT NULL COMMENT '结点类型',
  `nodeNo` int(11) NOT NULL COMMENT '结点编号（即结点id），建议同类结点顺序编号',
  `netLine` varchar(255) NOT NULL COMMENT '运营商线路',
  `ip` varchar(255) NOT NULL COMMENT 'ip',
  `port` int(11) NOT NULL COMMENT '服务端口',
  PRIMARY KEY (`name`,`nodeNo`),
  KEY `net_line_key` (`netLine`),
  KEY `nodeNo` (`nodeNo`,`name`),
  CONSTRAINT `name_key` FOREIGN KEY (`name`) REFERENCES `node_type` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `net_line_key` FOREIGN KEY (`netLine`) REFERENCES `net_line` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of node
-- ----------------------------
INSERT INTO `node` VALUES ('sns服务', '1', '移动', '192.168.1.105', '6702');
INSERT INTO `node` VALUES ('Tcp入口', '1', '电信', '192.168.1.105', '6601');
INSERT INTO `node` VALUES ('数据入口', '1', '电信', '192.168.1.105', '6901');
INSERT INTO `node` VALUES ('游戏服务', '1', '移动', '192.168.1.105', '6703');
INSERT INTO `node` VALUES ('认证服务', '1', '电信', '192.168.1.105', '6701');
INSERT INTO `node` VALUES ('通知服务', '1', '电信', '192.168.1.105', '6802');

-- ----------------------------
-- Table structure for node_type
-- ----------------------------
DROP TABLE IF EXISTS `node_type`;
CREATE TABLE `node_type` (
  `id` int(11) NOT NULL COMMENT '结点类型id= 模块Id',
  `name` varchar(255) DEFAULT NULL COMMENT '结点类型名',
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of node_type
-- ----------------------------
INSERT INTO `node_type` VALUES ('9', 'LBS服务');
INSERT INTO `node_type` VALUES ('7', 'sns服务');
INSERT INTO `node_type` VALUES ('2', 'Tcp入口');
INSERT INTO `node_type` VALUES ('5', '数据入口');
INSERT INTO `node_type` VALUES ('8', '游戏服务');
INSERT INTO `node_type` VALUES ('3', '认证服务');
INSERT INTO `node_type` VALUES ('1', '运维服务');
INSERT INTO `node_type` VALUES ('4', '通知服务');
