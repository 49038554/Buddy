/*
Navicat MySQL Data Transfer

Source Server         : sun
Source Server Version : 50150
Source Host           : 192.168.11.106:3306
Source Database       : Buddy

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2015-12-07 12:14:13
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `user_info`
-- ----------------------------
DROP TABLE IF EXISTS `user_info`;
CREATE TABLE `user_info` (
  `id` bigint(11) NOT NULL,
  `pwd` varchar(255) NOT NULL COMMENT 'md5(密码)',
  `randKey` varchar(255) DEFAULT 'NULL' COMMENT '随机key\r\nclient传输md5(明文密码)\r\nserver检查md5(md5(明文密码)+randKey)==pwd',
  `nickname` varchar(255) DEFAULT 'NULL' COMMENT '昵称',
  `signature` varchar(255) DEFAULT 'NULL' COMMENT '签名',
  `sex` int(11) DEFAULT '1' COMMENT '性别0女1男',
  `coin` int(11) DEFAULT '0' COMMENT '虚拟币',
  `headIco` varchar(255) DEFAULT 'NULL' COMMENT '个人头像',
  `level` smallint(8) DEFAULT '0' COMMENT '等级',
  `realName` varchar(40) DEFAULT 'NULL' COMMENT '真实姓名',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='用户表\r\n按照username首字母分片';

-- ----------------------------
-- Records of user_info
-- ----------------------------
