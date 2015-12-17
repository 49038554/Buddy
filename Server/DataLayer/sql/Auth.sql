/*
Navicat MySQL Data Transfer

Source Server         : sun
Source Server Version : 50150
Source Host           : 192.168.11.106:3306
Source Database       : Auth

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2015-12-17 15:42:06
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `bind_imei`
-- ----------------------------
DROP TABLE IF EXISTS `bind_imei`;
CREATE TABLE `bind_imei` (
  `imei` varchar(255) NOT NULL DEFAULT 'NULL' COMMENT 'imei',
  `id` bigint(11) DEFAULT '0' COMMENT '用户ID',
  PRIMARY KEY (`imei`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of bind_imei
-- ----------------------------

-- ----------------------------
-- Table structure for `bind_mobile`
-- ----------------------------
DROP TABLE IF EXISTS `bind_mobile`;
CREATE TABLE `bind_mobile` (
  `mobile` varchar(255) NOT NULL COMMENT '绑定手机',
  `id` bigint(11) DEFAULT NULL,
  PRIMARY KEY (`mobile`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='绑定手机表\r\n按照md5(mobile)前2字节分片';

-- ----------------------------
-- Records of bind_mobile
-- ----------------------------
