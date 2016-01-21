/*
Navicat MySQL Data Transfer

Source Server         : sun
Source Server Version : 50150
Source Host           : 192.168.11.106:3306
Source Database       : GameInit

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-01-21 16:30:21
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `buddy_book_info`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_book_info`;
CREATE TABLE `buddy_book_info` (
  `number` int(11) NOT NULL COMMENT '全国编号',
  `name` varchar(255) NOT NULL COMMENT '名字',
  `descript` varchar(60) NOT NULL COMMENT '描述,最大60byte',
  `race1` varchar(255) NOT NULL COMMENT '属性1',
  `race2` varchar(255) DEFAULT NULL COMMENT '属性2',
  `talent1` varchar(255) NOT NULL COMMENT '特性1',
  `talent2` varchar(255) DEFAULT NULL COMMENT '特性2',
  `talent3` varchar(255) DEFAULT NULL COMMENT '梦特性',
  `itemId` int(10) unsigned zerofill DEFAULT NULL COMMENT '专属物品，0没有专属物品',
  `hitPoint` int(11) NOT NULL COMMENT '血',
  `physicalA` int(11) NOT NULL COMMENT '物攻',
  `physicalD` int(11) NOT NULL COMMENT '物防',
  `specialA` int(11) NOT NULL COMMENT '特攻',
  `specialD` int(11) NOT NULL COMMENT '特防',
  `speed` int(11) NOT NULL COMMENT '速度',
  `rare` int(11) NOT NULL COMMENT '出现率 1~99',
  `tame` int(11) NOT NULL COMMENT '驯化率 1~99',
  `upMumber` int(10) unsigned zerofill DEFAULT NULL COMMENT '进化后编号，0无进化态',
  PRIMARY KEY (`number`),
  KEY `race1_key` (`race1`),
  KEY `race2_key` (`race2`),
  KEY `talent1_key` (`talent1`),
  KEY `talent2_key` (`talent2`),
  KEY `talent3_key` (`talent3`),
  KEY `name` (`name`),
  CONSTRAINT `race1_key` FOREIGN KEY (`race1`) REFERENCES `race` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `race2_key` FOREIGN KEY (`race2`) REFERENCES `race` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `talent1_key` FOREIGN KEY (`talent1`) REFERENCES `talent` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `talent2_key` FOREIGN KEY (`talent2`) REFERENCES `talent` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `talent3_key` FOREIGN KEY (`talent3`) REFERENCES `talent` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=gb2312 COMMENT='巴迪兽图鉴——资料表';

-- ----------------------------
-- Records of buddy_book_info
-- ----------------------------
INSERT INTO `buddy_book_info` VALUES ('1', '妙蛙种子', '啊', '草', null, '威吓', null, null, null, '1', '1', '1', '1', '1', '1', '1', '1', '0000000002');
INSERT INTO `buddy_book_info` VALUES ('2', '妙蛙花', '啊', '草', '毒', '威吓', null, null, null, '2', '2', '2', '2', '2', '2', '2', '2', '0000000003');

-- ----------------------------
-- Table structure for `buddy_book_skill`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_book_skill`;
CREATE TABLE `buddy_book_skill` (
  `buddy` varchar(255) NOT NULL COMMENT '巴迪兽名字',
  `skill` varchar(255) NOT NULL COMMENT '技能',
  `isInit` int(10) unsigned zerofill DEFAULT '0000000000' COMMENT '是否初始技能\r\n1是\r\n0不是',
  PRIMARY KEY (`buddy`,`skill`),
  KEY `skill_key` (`skill`),
  CONSTRAINT `buddy_key` FOREIGN KEY (`buddy`) REFERENCES `buddy_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `skill_key` FOREIGN KEY (`skill`) REFERENCES `skill_book` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=gb2312 COMMENT='巴迪兽图鉴——技能表';

-- ----------------------------
-- Records of buddy_book_skill
-- ----------------------------
INSERT INTO `buddy_book_skill` VALUES ('妙蛙花', '吞噬', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('妙蛙花', '摘星术', '0000000000');
INSERT INTO `buddy_book_skill` VALUES ('妙蛙种子', '光墙', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('妙蛙种子', '吞噬', '0000000000');
INSERT INTO `buddy_book_skill` VALUES ('妙蛙种子', '瓦割', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('妙蛙种子', '摘星术', '0000000000');

-- ----------------------------
-- Table structure for `buddy_map_lbs`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_map_lbs`;
CREATE TABLE `buddy_map_lbs` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `shape` int(11) NOT NULL COMMENT '形状：0圆形，1矩形',
  `x` double NOT NULL COMMENT '圆心/矩形顶点纬度',
  `y` double NOT NULL COMMENT '圆心/矩形顶点经度',
  `radius` int(11) NOT NULL COMMENT '半径，单位米',
  `right` double DEFAULT NULL COMMENT '矩形时有效，对角纬度',
  `bottom` double DEFAULT NULL COMMENT '矩形时有效，对角经度',
  `descript` varchar(255) DEFAULT NULL COMMENT '描述',
  `city` varchar(255) DEFAULT NULL COMMENT '城市',
  `spot` int(11) DEFAULT '0' COMMENT '是否景区 1是 0不是',
  PRIMARY KEY (`id`),
  KEY `city_name1` (`city`),
  CONSTRAINT `city_name1` FOREIGN KEY (`city`) REFERENCES `city` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=gb2312 COMMENT='巴迪分布地图 lbs信息';

-- ----------------------------
-- Records of buddy_map_lbs
-- ----------------------------
INSERT INTO `buddy_map_lbs` VALUES ('18', '1', '116.14164', '40.157777', '0', '116.308762', '39.998973', '上左', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('19', '1', '116.308762', '40.157777', '0', '116.472218', '39.998973', '上中', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('20', '1', '116.472218', '40.157777', '0', '116.654465', '39.998973', '上右', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('21', '1', '116.14164', '39.998973', '0', '116.308762', '39.866466', '中左', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('22', '0', '116.40305', '39.914912', '5000', null, null, '3环内', '北京', '1');
INSERT INTO `buddy_map_lbs` VALUES ('23', '1', '116.472218', '39.998973', '0', '116.654465', '39.866466', '中右', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('24', '1', '116.14164', '39.866466', '0', '116.308762', '39.751594', '下左', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('25', '1', '116.308762', '39.866466', '0', '116.472218', '39.751594', '下中', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('26', '1', '116.472218', '39.866466', '0', '116.654465', '39.751594', '下右', '北京', '0');
INSERT INTO `buddy_map_lbs` VALUES ('27', '0', '116.195074', '39.994998', '500', null, null, '香山', '北京', '1');
INSERT INTO `buddy_map_lbs` VALUES ('28', '0', '116.190187', '39.968073', '500', null, null, '八大处', '北京', '1');
INSERT INTO `buddy_map_lbs` VALUES ('29', '0', '116.216705', '40.00619', '250', null, null, '植物园', '北京', '1');
INSERT INTO `buddy_map_lbs` VALUES ('30', '0', '116.103519', '40.115447', '1000', null, null, '凤凰岭', '北京', '1');
INSERT INTO `buddy_map_lbs` VALUES ('31', '0', '116.653605', '40.026139', '1000', null, null, '东郊湿地公园', '北京', '1');

-- ----------------------------
-- Table structure for `buddy_map_pet`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_map_pet`;
CREATE TABLE `buddy_map_pet` (
  `id` int(11) NOT NULL COMMENT '地图id',
  `buddy` varchar(255) NOT NULL COMMENT '巴迪兽名字',
  PRIMARY KEY (`id`,`buddy`),
  KEY `buddy_key1` (`buddy`),
  CONSTRAINT `id_key1` FOREIGN KEY (`id`) REFERENCES `buddy_map_lbs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `buddy_key1` FOREIGN KEY (`buddy`) REFERENCES `buddy_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=gb2312 COMMENT='巴迪分布图 巴迪';

-- ----------------------------
-- Records of buddy_map_pet
-- ----------------------------
INSERT INTO `buddy_map_pet` VALUES ('18', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('19', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('20', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('22', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('24', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('26', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('28', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('30', '妙蛙花');
INSERT INTO `buddy_map_pet` VALUES ('18', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('20', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('21', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('23', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('25', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('27', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('29', '妙蛙种子');
INSERT INTO `buddy_map_pet` VALUES ('31', '妙蛙种子');

-- ----------------------------
-- Table structure for `city`
-- ----------------------------
DROP TABLE IF EXISTS `city`;
CREATE TABLE `city` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '城市id',
  `name` varchar(255) NOT NULL COMMENT '城市名',
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of city
-- ----------------------------
INSERT INTO `city` VALUES ('1', '北京');
INSERT INTO `city` VALUES ('2', '杭州');
INSERT INTO `city` VALUES ('3', '上海');
INSERT INTO `city` VALUES ('4', '武汉');

-- ----------------------------
-- Table structure for `ex_effect`
-- ----------------------------
DROP TABLE IF EXISTS `ex_effect`;
CREATE TABLE `ex_effect` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL COMMENT '名字',
  `descript` varchar(255) DEFAULT NULL COMMENT '描述',
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of ex_effect
-- ----------------------------
INSERT INTO `ex_effect` VALUES ('1', '烧伤', '攻击减半');
INSERT INTO `ex_effect` VALUES ('2', '麻痹', '速度降低75%');
INSERT INTO `ex_effect` VALUES ('3', '冰冻', '不能行动0~6回合，每回合25%几率解除');
INSERT INTO `ex_effect` VALUES ('4', '睡眠', '不能行动0~6回合，每回合25%几率解除');
INSERT INTO `ex_effect` VALUES ('5', '中毒', '每回合损失最大体力8%');
INSERT INTO `ex_effect` VALUES ('6', '虚弱', '物攻降1级');
INSERT INTO `ex_effect` VALUES ('7', '破甲', '物防降1级');
INSERT INTO `ex_effect` VALUES ('8', '缺氧', '特攻降1级');
INSERT INTO `ex_effect` VALUES ('9', '破魔', '特防降1级');
INSERT INTO `ex_effect` VALUES ('10', '残废', '速度降1级');

-- ----------------------------
-- Table structure for `item_book`
-- ----------------------------
DROP TABLE IF EXISTS `item_book`;
CREATE TABLE `item_book` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `name` varchar(14) NOT NULL COMMENT '名字',
  `coin` int(11) NOT NULL COMMENT '价值',
  `descript` varchar(60) NOT NULL COMMENT '描述',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=gb2312 COMMENT='物品图鉴';

-- ----------------------------
-- Records of item_book
-- ----------------------------
INSERT INTO `item_book` VALUES ('1', '上帝之手', '50', '使用摘星术必须品，可摘取星空中的法宝');
INSERT INTO `item_book` VALUES ('2', '万能药', '50', '解除所有异常');
INSERT INTO `item_book` VALUES ('3', '人参', '100', '补满血');
INSERT INTO `item_book` VALUES ('4', '巴迪球', '0', '驯化巴迪兽');
INSERT INTO `item_book` VALUES ('5', '兴奋剂', '200', '战斗中临时增加能力');
INSERT INTO `item_book` VALUES ('6', '天地灵根', '100000', '每日生长出日用品');
INSERT INTO `item_book` VALUES ('7', '专爱头巾', '50000', '携带后，能力*1.5，不能换技能');
INSERT INTO `item_book` VALUES ('8', '生命玉', '30000', '携带后，能力*1.25，每次攻击受到8%反弹');
INSERT INTO `item_book` VALUES ('9', '恶魔果实', '10000', '吃下后，学会技能');
INSERT INTO `item_book` VALUES ('10', '进化果实', '1000', '吃下后，进化');

-- ----------------------------
-- Table structure for `race`
-- ----------------------------
DROP TABLE IF EXISTS `race`;
CREATE TABLE `race` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(11) NOT NULL COMMENT '属性名',
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of race
-- ----------------------------
INSERT INTO `race` VALUES ('5', '冰');
INSERT INTO `race` VALUES ('12', '草');
INSERT INTO `race` VALUES ('8', '超');
INSERT INTO `race` VALUES ('11', '虫');
INSERT INTO `race` VALUES ('13', '地');
INSERT INTO `race` VALUES ('4', '电');
INSERT INTO `race` VALUES ('16', '斗');
INSERT INTO `race` VALUES ('17', '毒');
INSERT INTO `race` VALUES ('7', '恶');
INSERT INTO `race` VALUES ('10', '飞');
INSERT INTO `race` VALUES ('15', '钢');
INSERT INTO `race` VALUES ('6', '鬼');
INSERT INTO `race` VALUES ('2', '火');
INSERT INTO `race` VALUES ('9', '龙');
INSERT INTO `race` VALUES ('1', '普');
INSERT INTO `race` VALUES ('3', '水');
INSERT INTO `race` VALUES ('14', '岩');

-- ----------------------------
-- Table structure for `skill_book`
-- ----------------------------
DROP TABLE IF EXISTS `skill_book`;
CREATE TABLE `skill_book` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(14) NOT NULL COMMENT '名字，最大7汉字',
  `race` varchar(11) NOT NULL COMMENT '属性',
  `power` int(11) NOT NULL COMMENT '威力0~300',
  `isPhysical` int(11) DEFAULT '1' COMMENT '是否物理攻击：\r\n1是 0不是',
  `hitRate` int(11) DEFAULT '100' COMMENT '命中率30~101， 101为必中技能',
  `isMapSkill` int(11) unsigned zerofill DEFAULT '00000000000' COMMENT '是否地图技能：\r\n1是 0不是',
  `priority` int(10) unsigned zerofill DEFAULT '0000000000' COMMENT '先手级别0~6',
  `descript` varchar(60) NOT NULL COMMENT '描述，最大60byte',
  `exEffect` varchar(255) DEFAULT NULL COMMENT '特效',
  PRIMARY KEY (`id`),
  KEY `race_name` (`race`),
  KEY `name` (`name`),
  KEY `exEffect_name` (`exEffect`),
  CONSTRAINT `exEffect_name` FOREIGN KEY (`exEffect`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `race_name` FOREIGN KEY (`race`) REFERENCES `race` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=gb2312 COMMENT='技能图鉴';

-- ----------------------------
-- Records of skill_book
-- ----------------------------
INSERT INTO `skill_book` VALUES ('10', '光墙', '超', '0', '0', '100', '00000000001', '0000000000', '制造一堵能量墙特殊伤害减半。非战斗使用可以建造房屋', null);
INSERT INTO `skill_book` VALUES ('11', '摘星术', '普', '180', '1', '100', '00000000001', '0000000000', '使用亿万冲击波攻击对手，非战斗使用可摘取星空中的法宝', null);
INSERT INTO `skill_book` VALUES ('12', '吞噬', '恶', '0', '0', '100', '00000000001', '0000000000', '使用强大吞噬力吸住对手，防止逃跑，换人，非战斗使用可以吞掉物品，法宝转换为正能量', null);
INSERT INTO `skill_book` VALUES ('13', '瓦割', '斗', '75', '1', '100', '00000000001', '0000000000', '使用锋利爪子攻击对手，可破坏光墙，反射盾，非战斗使用可拆除建筑物', null);

-- ----------------------------
-- Table structure for `talent`
-- ----------------------------
DROP TABLE IF EXISTS `talent`;
CREATE TABLE `talent` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL COMMENT '特性名',
  `descript` varchar(255) NOT NULL COMMENT '特性描述',
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `name_2` (`name`),
  KEY `name_3` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of talent
-- ----------------------------
INSERT INTO `talent` VALUES ('1', '威吓', '对放物理攻击降低1级');
INSERT INTO `talent` VALUES ('2', '石脑', '不受反弹伤害');
