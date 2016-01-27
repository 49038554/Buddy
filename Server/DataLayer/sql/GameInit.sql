/*
Navicat MySQL Data Transfer

Source Server         : sun
Source Server Version : 50150
Source Host           : 192.168.11.106:3306
Source Database       : GameInit

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-01-27 16:55:22
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
INSERT INTO `buddy_book_skill` VALUES ('妙蛙种子', '清新', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('妙蛙种子', '吞噬', '0000000000');
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
) ENGINE=InnoDB AUTO_INCREMENT=140 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of ex_effect
-- ----------------------------
INSERT INTO `ex_effect` VALUES ('1', '烧伤', '攻击减半');
INSERT INTO `ex_effect` VALUES ('2', '麻痹', '速度降低75%');
INSERT INTO `ex_effect` VALUES ('3', '冰冻', '不能行动0~6回合，每回合25%几率解除');
INSERT INTO `ex_effect` VALUES ('4', '催眠', '不能行动0~6回合，每回合25%几率解除');
INSERT INTO `ex_effect` VALUES ('5', '中毒', '每回合损失最大体力8%');
INSERT INTO `ex_effect` VALUES ('11', '1击毙', '1击毙');
INSERT INTO `ex_effect` VALUES ('12', '睡3回合，HP全满，状态解除', '睡3回合，HP全满，状态解除');
INSERT INTO `ex_effect` VALUES ('13', '混乱', '50%攻击自己');
INSERT INTO `ex_effect` VALUES ('14', '每回合吸收1/8体力', '每回合吸收1/8体力');
INSERT INTO `ex_effect` VALUES ('15', '禁止换人和逃跑', '禁止换人和逃跑');
INSERT INTO `ex_effect` VALUES ('16', 'HP越低威力越大20~200', null);
INSERT INTO `ex_effect` VALUES ('17', '交换物品', null);
INSERT INTO `ex_effect` VALUES ('18', '不能连续使用相同技能', null);
INSERT INTO `ex_effect` VALUES ('19', '3回合不能使用变化技能', null);
INSERT INTO `ex_effect` VALUES ('20', '对方HP减少到与自己相同', null);
INSERT INTO `ex_effect` VALUES ('21', '对方下回合被催眠', null);
INSERT INTO `ex_effect` VALUES ('23', '非战斗使用可引出野生巴迪', null);
INSERT INTO `ex_effect` VALUES ('24', '3回合不能更换技能', null);
INSERT INTO `ex_effect` VALUES ('25', '双方HP平分', null);
INSERT INTO `ex_effect` VALUES ('26', '损失1/4HP制造替身', null);
INSERT INTO `ex_effect` VALUES ('32', '提高闪避', null);
INSERT INTO `ex_effect` VALUES ('33', '提高命中', null);
INSERT INTO `ex_effect` VALUES ('34', '强化物攻', null);
INSERT INTO `ex_effect` VALUES ('35', '强化物防', null);
INSERT INTO `ex_effect` VALUES ('36', '强化特攻', null);
INSERT INTO `ex_effect` VALUES ('37', '强化特防', null);
INSERT INTO `ex_effect` VALUES ('38', '强化速度', null);
INSERT INTO `ex_effect` VALUES ('39', '提高暴击率', null);
INSERT INTO `ex_effect` VALUES ('40', '极限物攻', null);
INSERT INTO `ex_effect` VALUES ('41', '弱化物攻', null);
INSERT INTO `ex_effect` VALUES ('42', '弱化物防', null);
INSERT INTO `ex_effect` VALUES ('43', '弱化特攻', null);
INSERT INTO `ex_effect` VALUES ('44', '弱化特防', null);
INSERT INTO `ex_effect` VALUES ('45', '弱化速度', null);
INSERT INTO `ex_effect` VALUES ('47', '全体能力等级复原', null);
INSERT INTO `ex_effect` VALUES ('48', '全体异常解除', null);
INSERT INTO `ex_effect` VALUES ('49', '回复50%HP,晴天2倍，其它天气减半', null);
INSERT INTO `ex_effect` VALUES ('52', '对方不能使用我方会的技能', null);
INSERT INTO `ex_effect` VALUES ('56', '使上场巴迪受到1/8伤害', null);
INSERT INTO `ex_effect` VALUES ('57', '电系威力*1.5', null);
INSERT INTO `ex_effect` VALUES ('60', '预留', null);
INSERT INTO `ex_effect` VALUES ('61', '非战斗使用可以吞掉物品，法宝转换为正能量', null);
INSERT INTO `ex_effect` VALUES ('62', '交换巴迪', null);
INSERT INTO `ex_effect` VALUES ('63', '回复50%HP', null);
INSERT INTO `ex_effect` VALUES ('64', '拉对手一起死', null);
INSERT INTO `ex_effect` VALUES ('66', '强制退场', null);
INSERT INTO `ex_effect` VALUES ('68', '普和斗属性技能可打中鬼系巴迪', null);
INSERT INTO `ex_effect` VALUES ('69', '解除异常', null);
INSERT INTO `ex_effect` VALUES ('70', '吸引全场火力', null);
INSERT INTO `ex_effect` VALUES ('72', '交换巴迪', null);
INSERT INTO `ex_effect` VALUES ('73', '睡着时，随机使用技能', null);
INSERT INTO `ex_effect` VALUES ('74', '不会被打死，连用成功率降低', null);
INSERT INTO `ex_effect` VALUES ('75', '下回合技能必中', null);
INSERT INTO `ex_effect` VALUES ('76', '永久复制对手技能', null);
INSERT INTO `ex_effect` VALUES ('77', '听过的巴迪，3回合内不换下，则濒死', null);
INSERT INTO `ex_effect` VALUES ('78', '预留', null);
INSERT INTO `ex_effect` VALUES ('79', '对方攻击无效，连用成功率降低', null);
INSERT INTO `ex_effect` VALUES ('80', '变成对手', null);
INSERT INTO `ex_effect` VALUES ('81', '预留', null);
INSERT INTO `ex_effect` VALUES ('86', '眩晕', null);
INSERT INTO `ex_effect` VALUES ('87', '先手', null);
INSERT INTO `ex_effect` VALUES ('88', '暴击', null);
INSERT INTO `ex_effect` VALUES ('89', '吸血', null);
INSERT INTO `ex_effect` VALUES ('90', '反噬', null);
INSERT INTO `ex_effect` VALUES ('91', '再攻击1次', null);
INSERT INTO `ex_effect` VALUES ('92', '下回合不能行动', null);
INSERT INTO `ex_effect` VALUES ('93', '无盲点', null);
INSERT INTO `ex_effect` VALUES ('94', 'AOE', null);
INSERT INTO `ex_effect` VALUES ('95', '乱舞', '连续攻击2~3回合，然后混乱');
INSERT INTO `ex_effect` VALUES ('96', '非战斗使用可拆除建筑物', null);
INSERT INTO `ex_effect` VALUES ('97', '具有飞行属性伤害', null);
INSERT INTO `ex_effect` VALUES ('98', '无视对方能力变化', null);
INSERT INTO `ex_effect` VALUES ('99', '100%发动特效', null);
INSERT INTO `ex_effect` VALUES ('100', '对自己发动特效', null);
INSERT INTO `ex_effect` VALUES ('101', '蓄力', '被打断则失败');
INSERT INTO `ex_effect` VALUES ('102', '后手', null);
INSERT INTO `ex_effect` VALUES ('104', '物理伤害2倍返还', null);
INSERT INTO `ex_effect` VALUES ('105', '击倒对手时候发动特效', null);
INSERT INTO `ex_effect` VALUES ('106', '对方已受伤害时发动特效', null);
INSERT INTO `ex_effect` VALUES ('107', '威力翻倍', null);
INSERT INTO `ex_effect` VALUES ('108', '给敌人留下至少1点体力', null);
INSERT INTO `ex_effect` VALUES ('109', '不能更换技能', null);
INSERT INTO `ex_effect` VALUES ('111', '对方中毒时发动特效', null);
INSERT INTO `ex_effect` VALUES ('112', '太阳能', '不是晴天，需要蓄力1回合');
INSERT INTO `ex_effect` VALUES ('113', '攻击延后2回合', '攻击延后2回合');
INSERT INTO `ex_effect` VALUES ('114', '物理伤害', null);
INSERT INTO `ex_effect` VALUES ('115', '非战斗使用可摘取星空中的法宝', null);
INSERT INTO `ex_effect` VALUES ('116', '需要上帝之手', null);
INSERT INTO `ex_effect` VALUES ('117', '全体攻击', null);
INSERT INTO `ex_effect` VALUES ('118', '非战斗使用建造秘密基地', null);
INSERT INTO `ex_effect` VALUES ('119', '自身异常时发动特效', null);
INSERT INTO `ex_effect` VALUES ('120', '清除钉子，减伤，天气等场地效果', '清除钉子，减伤，天气等场地效果');
INSERT INTO `ex_effect` VALUES ('121', '5回合冰雹天气，冰属性以外的巴迪每回合开始受到1/16伤害', null);
INSERT INTO `ex_effect` VALUES ('126', '5回合晴天', '');
INSERT INTO `ex_effect` VALUES ('129', '5回合雨天', '');
INSERT INTO `ex_effect` VALUES ('130', '5回合沙暴天气，地岩钢以外属性巴迪受到1/16伤害', '');
INSERT INTO `ex_effect` VALUES ('131', '5回合特殊伤害减半', '');
INSERT INTO `ex_effect` VALUES ('132', '5回合物理伤害减半', '');
INSERT INTO `ex_effect` VALUES ('133', '5回合浮游与飞行系能被地系技能打中', '');
INSERT INTO `ex_effect` VALUES ('134', '5回合速度慢的先行动', '');
INSERT INTO `ex_effect` VALUES ('135', '5回合不被命中要害', '');
INSERT INTO `ex_effect` VALUES ('136', '5回合，每回合回复8%HP', '');
INSERT INTO `ex_effect` VALUES ('137', '不受反噬', null);
INSERT INTO `ex_effect` VALUES ('138', '不被弱化', null);

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
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=gb2312;

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
INSERT INTO `race` VALUES ('18', '妖');

-- ----------------------------
-- Table structure for `skill_book`
-- ----------------------------
DROP TABLE IF EXISTS `skill_book`;
CREATE TABLE `skill_book` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(14) NOT NULL COMMENT '名字，最大7汉字',
  `race` varchar(11) NOT NULL COMMENT '属性',
  `power` int(11) NOT NULL COMMENT '威力0~300',
  `type` int(11) DEFAULT '1' COMMENT '技能类型：\r\n1物理 2变化 3特殊',
  `hitRate` int(11) DEFAULT '100' COMMENT '命中率30~101， 101为必中技能',
  `isMapSkill` int(11) unsigned zerofill DEFAULT '00000000000' COMMENT '是否地图技能：\r\n1是 0不是',
  `priority` int(10) unsigned zerofill DEFAULT '0000000000' COMMENT '先手级别0~6',
  `descript` varchar(60) NOT NULL COMMENT '描述，最大60byte',
  `effect1` varchar(255) DEFAULT NULL,
  `effect2` varchar(255) DEFAULT NULL,
  `effect3` varchar(255) DEFAULT NULL,
  `effect4` varchar(255) DEFAULT NULL,
  `effect5` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `race_name` (`race`),
  KEY `name` (`name`),
  KEY `effect2_name` (`effect2`),
  KEY `effect3_name` (`effect3`),
  KEY `effect4_name` (`effect4`),
  KEY `effect5_name` (`effect5`),
  KEY `effect1_name` (`effect1`),
  CONSTRAINT `effect1_name` FOREIGN KEY (`effect1`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `effect2_name` FOREIGN KEY (`effect2`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `effect3_name` FOREIGN KEY (`effect3`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `effect4_name` FOREIGN KEY (`effect4`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `effect5_name` FOREIGN KEY (`effect5`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `race_name` FOREIGN KEY (`race`) REFERENCES `race` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=538 DEFAULT CHARSET=gb2312 COMMENT='技能图鉴';

-- ----------------------------
-- Records of skill_book
-- ----------------------------
INSERT INTO `skill_book` VALUES ('10', '秘密力量', '普', '70', '3', '100', '00000000001', '0000000000', '非战斗使用可以建造秘密基地', '非战斗使用建造秘密基地', null, null, null, null);
INSERT INTO `skill_book` VALUES ('11', '摘星术', '普', '180', '3', '100', '00000000001', '0000000000', '使用亿万冲击波攻击对手，非战斗使用可摘取星空中的法宝', '需要上帝之手', '非战斗使用可摘取星空中的法宝', null, null, null);
INSERT INTO `skill_book` VALUES ('12', '吞噬', '恶', '0', '2', '101', '00000000001', '0000000000', '', '禁止换人和逃跑', '非战斗使用可以吞掉物品，法宝转换为正能量', null, null, null);
INSERT INTO `skill_book` VALUES ('14', '逆刃刀', '普', '40', '1', '100', '00000000000', '0000000000', '敌人至少留下1点体力', '给敌人留下至少1点体力', null, null, null, null);
INSERT INTO `skill_book` VALUES ('15', '爆音波', '普', '90', '3', '100', '00000000000', '0000000000', '全体攻击', '全体攻击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('16', '吼叫', '普', '0', '2', '101', '00000000000', '0000000000', '', '后手', '强制退场', null, null, null);
INSERT INTO `skill_book` VALUES ('19', '舍身撞', '普', '120', '1', '100', '00000000000', '0000000000', '8%反伤', '反噬', null, null, null, null);
INSERT INTO `skill_book` VALUES ('26', '亿万冲击', '普', '150', '1', '90', '00000000000', '0000000000', '1回合不能行动，可换人', '下回合不能行动', null, null, null, null);
INSERT INTO `skill_book` VALUES ('28', '最终手段', '普', '160', '1', '100', '00000000000', '0000000000', '', '对自己发动特效', '不能更换技能', '禁止换人和逃跑', '弱化物防', '弱化特防');
INSERT INTO `skill_book` VALUES ('30', '幸运光环', '普', '0', '2', '101', '00000000000', '0000000000', '', '5回合不被命中要害', null, null, null, null);
INSERT INTO `skill_book` VALUES ('34', '吠叫', '普', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('35', '黑眼', '普', '0', '2', '101', '00000000000', '0000000000', '', '禁止换人和逃跑', null, null, null, null);
INSERT INTO `skill_book` VALUES ('36', '嗅觉', '普', '0', '2', '101', '00000000000', '0000000000', '', '普和斗属性技能可打中鬼系巴迪', null, null, null, null);
INSERT INTO `skill_book` VALUES ('37', '崩击爪', '普', '75', '1', '95', '00000000000', '0000000000', '30%几率物防-1', '弱化物防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('39', '偷懒', '普', '0', '2', '101', '00000000000', '0000000000', '', '回复50%HP', null, null, null, null);
INSERT INTO `skill_book` VALUES ('41', '清新', '普', '0', '2', '101', '00000000000', '0000000000', '', '解除异常', null, null, null, null);
INSERT INTO `skill_book` VALUES ('42', '莽撞', '普', '0', '2', '100', '00000000000', '0000000000', '', '对方HP减少到与自己相同', null, null, null, null);
INSERT INTO `skill_book` VALUES ('43', '哈欠', '普', '0', '2', '100', '00000000000', '0000000000', '', '对方下回合被催眠', null, null, null, null);
INSERT INTO `skill_book` VALUES ('45', '向我开炮', '普', '0', '2', '101', '00000000001', '0000000000', '', '吸引全场火力', '非战斗使用可引出野生巴迪', null, null, null);
INSERT INTO `skill_book` VALUES ('47', '空元气', '普', '70', '1', '100', '00000000000', '0000000000', '自身异常状态时威力*2', '自身异常时发动特效', '威力翻倍', null, null, null);
INSERT INTO `skill_book` VALUES ('49', '假动作', '普', '40', '1', '100', '00000000000', '0000000000', '先制+1，使对方害怕，上场的第一回合才能使用', '100%发动特效', '眩晕', null, null, null);
INSERT INTO `skill_book` VALUES ('50', '神速', '普', '80', '1', '100', '00000000000', '0000000000', '先制+3', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('52', '晨光', '普', '0', '2', '101', '00000000000', '0000000000', '', '回复50%HP,晴天2倍，其它天气减半', null, null, null, null);
INSERT INTO `skill_book` VALUES ('54', '清场', '普', '20', '1', '100', '00000000000', '0000000000', '清除钉子，种子，缠绕', '清除钉子，减伤，天气等场地效果', null, null, null, null);
INSERT INTO `skill_book` VALUES ('55', '鼓掌', '普', '0', '2', '100', '00000000000', '0000000000', '', '3回合不能更换技能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('57', '分享痛楚', '普', '0', '2', '100', '00000000000', '0000000000', '', '双方HP平分', null, null, null, null);
INSERT INTO `skill_book` VALUES ('59', '治愈铃声', '普', '0', '2', '101', '00000000000', '0000000000', '', '全体异常解除', null, null, null, null);
INSERT INTO `skill_book` VALUES ('60', '梦话', '普', '0', '2', '101', '00000000000', '0000000000', '', '睡着时，随机使用技能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('61', '吃奶', '普', '0', '2', '101', '00000000000', '0000000000', '', '回复50%HP', null, null, null, null);
INSERT INTO `skill_book` VALUES ('63', '忍耐', '普', '0', '2', '101', '00000000000', '0000000000', '', '不会被打死，连用成功率降低', null, null, null, null);
INSERT INTO `skill_book` VALUES ('64', '心眼', '普', '0', '2', '101', '00000000000', '0000000000', '', '下回合技能必中', null, null, null, null);
INSERT INTO `skill_book` VALUES ('65', '素描', '普', '0', '2', '101', '00000000000', '0000000000', '', '永久复制对手技能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('66', '末日歌', '普', '0', '2', '101', '00000000000', '0000000000', '', '听过的巴迪，3回合内不换下，则濒死', null, null, null, null);
INSERT INTO `skill_book` VALUES ('67', '吃撑了', '普', '0', '2', '101', '00000000000', '0000000000', '', '极限物攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('68', '保护', '普', '0', '2', '101', '00000000000', '0000000000', '', '对方攻击无效，连用成功率降低', null, null, null, null);
INSERT INTO `skill_book` VALUES ('69', '垂死挣扎', '普', '0', '1', '100', '00000000000', '0000000000', '', 'HP越低威力越大20~200', null, null, null, null);
INSERT INTO `skill_book` VALUES ('70', '替身', '普', '0', '2', '100', '00000000000', '0000000000', '', '损失1/4HP制造替身', null, null, null, null);
INSERT INTO `skill_book` VALUES ('71', '劈开', '普', '70', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('72', '三角攻击', '普', '70', '3', '100', '00000000000', '0000000000', '30%几率烧伤，麻痹，冰冻', '麻痹', '烧伤', '冰冻', null, null);
INSERT INTO `skill_book` VALUES ('73', '必杀门牙', '普', '80', '1', '90', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('74', '大爆炸', '普', '250', '1', '100', '00000000000', '0000000000', '濒死', '对自己发动特效', '1击毙', null, null, null);
INSERT INTO `skill_book` VALUES ('75', '昏迷拳', '普', '70', '1', '100', '00000000000', '0000000000', '30%混乱', '混乱', null, null, null, null);
INSERT INTO `skill_book` VALUES ('76', '变身', '普', '0', '2', '101', '00000000000', '0000000000', '', '变成对手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('77', '蛇瞪眼', '普', '0', '2', '100', '00000000000', '0000000000', '', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('78', '恶魔吻', '普', '0', '2', '75', '00000000000', '0000000000', '', '催眠', null, null, null, null);
INSERT INTO `skill_book` VALUES ('79', '高速星星', '普', '60', '3', '101', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('80', '自爆', '普', '200', '1', '100', '00000000000', '0000000000', '濒死', '对自己发动特效', '1击毙', null, null, null);
INSERT INTO `skill_book` VALUES ('81', '聚气', '普', '0', '2', '101', '00000000000', '0000000000', '', '提高暴击率', null, null, null, null);
INSERT INTO `skill_book` VALUES ('82', '硬梆梆', '普 ', '0', '2', '100', '00000000000', '0000000000', '', '强化物防', '强化物防', null, null, null);
INSERT INTO `skill_book` VALUES ('84', '电光石火', '普', '40', '1', '100', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('85', '生长', '普', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', '强化特攻', null, null, null);
INSERT INTO `skill_book` VALUES ('86', '怪力', '普', '90', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('87', '破坏死光', '普', '150', '3', '90', '00000000000', '0000000000', '1回合不能行动，可以换人', '下回合不能行动', null, null, null, null);
INSERT INTO `skill_book` VALUES ('88', '横冲直撞', '普', '120', '1', '100', '00000000000', '0000000000', '连续使用2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('89', '泰山压顶', '普', '80', '1', '100', '00000000000', '0000000000', '30%麻痹', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('90', '剪刀断头台', '普', '0', '2', '30', '00000000000', '0000000000', '', '1击毙', null, null, null, null);
INSERT INTO `skill_book` VALUES ('92', '铁头功', '普', '70', '1', '100', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('93', '百万飞踢', '普', '120', '1', '85', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('95', '旋风', '普', '0', '2', '101', '00000000000', '0000000000', '', '后手', '强制退场', null, null, null);
INSERT INTO `skill_book` VALUES ('96', '剑舞', '普', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', '强化物攻', null, null, null);
INSERT INTO `skill_book` VALUES ('97', '旋风腿', '普', '40', '1', '100', '00000000000', '0000000000', '攻击2次，CT+1', '暴击', '再攻击1次', null, null, null);
INSERT INTO `skill_book` VALUES ('98', '百万拳击', '普', '100', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('99', '睡觉', '超', '0', '2', '101', '00000000000', '0000000000', '', '睡3回合，HP全满，状态解除', null, null, null, null);
INSERT INTO `skill_book` VALUES ('100', '双重重击', '普', '60', '1', '95', '00000000000', '0000000000', '攻击2次', '再攻击1次', null, null, null, null);
INSERT INTO `skill_book` VALUES ('101', '天使吻', '妖', '0', '2', '75', '00000000000', '0000000000', '', '混乱', null, null, null, null);
INSERT INTO `skill_book` VALUES ('102', '月光', '妖', '0', '2', '101', '00000000000', '0000000000', '', '回复50%HP,晴天2倍，其它天气减半', null, null, null, null);
INSERT INTO `skill_book` VALUES ('104', '吸精', '妖', '75', '1', '100', '00000000000', '0000000000', '伤害75%回复自身', '吸血', null, null, null, null);
INSERT INTO `skill_book` VALUES ('107', '嬉戏', '妖', '90', '1', '90', '00000000000', '0000000000', '30%混乱', '混乱', null, null, null, null);
INSERT INTO `skill_book` VALUES ('108', '妖风', '妖', '40', '3', '100', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('110', '月影斩', '妖', '95', '3', '100', '00000000000', '0000000000', '特攻+1', '强化特攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('111', '破灭之光', '妖', '140', '3', '85', '00000000000', '0000000000', '8%反伤', '反噬', null, null, null, null);
INSERT INTO `skill_book` VALUES ('120', '山岚', '斗', '60', '1', '100', '00000000000', '0000000000', '100%CT', '100%发动特效', '暴击', null, null, null);
INSERT INTO `skill_book` VALUES ('121', '冲击波', '斗', '120', '3', '85', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('124', '波动拳', '斗', '90', '3', '90', '00000000000', '0000000000', '30%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('126', '近身击', '斗', '120', '1', '100', '00000000000', '0000000000', '双防-1', '对自己发动特效', '弱化物防', '弱化特防', null, null);
INSERT INTO `skill_book` VALUES ('129', '巨大化', '斗', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', '强化物防', null, null, null);
INSERT INTO `skill_book` VALUES ('130', '升龙拳', '斗', '90', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('133', '气合拳', '斗', '150', '1', '100', '00000000000', '0000000000', '先制-3，出招前受到攻击则失败', '后手', '蓄力', null, null, null);
INSERT INTO `skill_book` VALUES ('134', '裸奔气合拳', '斗', '200', '1', '100', '00000000000', '0000000000', '先制-3，出招前受到攻击则失败', '后手', '蓄力', '对自己发动特效', '强制退场', null);
INSERT INTO `skill_book` VALUES ('135', '碎石踢', '斗', '80', '1', '90', '00000000000', '0000000001', '', '眩晕', '非战斗使用可拆除建筑物', null, null, null);
INSERT INTO `skill_book` VALUES ('136', '十字切', '斗', '80', '1', '85', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('138', '音速拳', '斗', '40', '1', '100', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('139', '起死回生', '斗', '0', '1', '100', '00000000000', '0000000000', '', 'HP越低威力越大20~200', null, null, null, null);
INSERT INTO `skill_book` VALUES ('143', '魔火', '火', '95', '3', '100', '00000000000', '0000000000', '30%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('147', '燃烧殆尽', '火', '150', '3', '90', '00000000000', '0000000000', '特攻-2', '对自己发动特效', '弱化特攻', '弱化特攻', '全体攻击', null);
INSERT INTO `skill_book` VALUES ('151', '烈焰突袭', '火', '130', '1', '95', '00000000000', '0000000000', '8%反伤，10%烧伤', '烧伤', '反噬', null, null, null);
INSERT INTO `skill_book` VALUES ('155', '鬼火', '火', '0', '2', '95', '00000000000', '0000000000', '', '烧伤', null, null, null, null);
INSERT INTO `skill_book` VALUES ('157', '晴天', '火', '0', '2', '101', '00000000000', '0000000000', '', '5回合晴天', null, null, null, null);
INSERT INTO `skill_book` VALUES ('160', '大字爆', '火', '125', '3', '80', '00000000000', '0000000000', '30%烧伤', '烧伤', '全体攻击', null, null, null);
INSERT INTO `skill_book` VALUES ('162', '喷射火焰', '火', '105', '3', '100', '00000000000', '0000000000', '20%烧伤', '烧伤', '全体攻击', null, null, null);
INSERT INTO `skill_book` VALUES ('163', '火焰拳', '火', '100', '1', '100', '00000000000', '0000000000', '30%烧伤', '烧伤', null, null, null, null);
INSERT INTO `skill_book` VALUES ('164', '雪崩', '冰', '85', '1', '95', '00000000000', '0000000000', '30%几率使对方害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('165', '暗香突袭', '冰', '60', '3', '100', '00000000000', '0000000000', 'CT', '100%发动特效', '暴击', null, null, null);
INSERT INTO `skill_book` VALUES ('167', '冰刀', '冰', '40', '1', '100', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('169', '绝对零度', '冰', '0', '1', '30', '00000000000', '0000000000', '1击毙', '1击毙', null, null, null, null);
INSERT INTO `skill_book` VALUES ('170', '冰雹', '冰', '0', '2', '101', '00000000000', '0000000000', '', '5回合冰雹天气，冰属性以外的巴迪每回合开始受到1/16伤害', null, null, null, null);
INSERT INTO `skill_book` VALUES ('172', '细雪', '冰', '40', '3', '100', '00000000000', '0000000000', '30%冰封', '冰冻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('173', '黑雾', '冰', '0', '2', '101', '00000000000', '0000000000', '', '全体能力等级复原', null, null, null, null);
INSERT INTO `skill_book` VALUES ('174', '极光', '冰', '80', '3', '100', '00000000000', '0000000000', '30%速度-1', '弱化速度', null, null, null, null);
INSERT INTO `skill_book` VALUES ('175', '暴风雪', '冰', '120', '3', '80', '00000000000', '0000000000', '30%冰封', '冰冻', '全体攻击', null, null, null);
INSERT INTO `skill_book` VALUES ('176', '急冻光线', '冰', '95', '3', '100', '00000000000', '0000000000', '20%冰封', '冰冻', '全体攻击', null, null, null);
INSERT INTO `skill_book` VALUES ('177', '急冻拳', '冰', '80', '1', '100', '00000000000', '0000000000', '30%冰封', '冰冻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('181', '沸水', '水', '80', '3', '100', '00000000000', '0000000000', '30%烧伤', '烧伤', null, null, null, null);
INSERT INTO `skill_book` VALUES ('183', '鲤鱼摆尾', '水', '80', '1', '100', '00000000000', '0000000000', '强制换人，野战结束战斗', '乱舞', '强制退场', null, null, null);
INSERT INTO `skill_book` VALUES ('187', '急救液', '水', '0', '2', '101', '00000000000', '0000000000', '', '5回合，每回合回复8%HP', null, null, null, null);
INSERT INTO `skill_book` VALUES ('192', '高压水炮', '水', '140', '1', '90', '00000000000', '0000000000', '下回合不能性能，可以换人', '下回合不能行动', null, null, null, null);
INSERT INTO `skill_book` VALUES ('193', '潜水', '水', '60', '1', '100', '00000000000', '0000000000', 'CT', '100%发动特效', '暴击', null, null, null);
INSERT INTO `skill_book` VALUES ('195', '求雨', '水', '0', '2', '101', '00000000000', '0000000000', '', '5回合雨天', null, null, null, null);
INSERT INTO `skill_book` VALUES ('197', '出水一击', '水', '80', '1', '100', '00000000000', '0000000000', '使用对方害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('198', '泡泡', '水', '40', '3', '100', '00000000000', '0000000000', '30%速度-1', '弱化速度', null, null, null, null);
INSERT INTO `skill_book` VALUES ('199', '跃龙门', '水', '100', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('200', '缩壳', '水', '0', '2', '101', '00000000000', '0000000000', '', '强化物防', '强化物防', null, null, null);
INSERT INTO `skill_book` VALUES ('202', '冲浪', '水', '95', '3', '100', '00000000000', '0000000000', '', '全体攻击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('203', '激流涌进', '水', '138', '1', '100', '00000000000', '0000000000', '连续使用3~5回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('204', '电磁波', '电', '0', '2', '100', '00000000000', '0000000000', '', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('205', '打雷', '电', '120', '3', '80', '00000000000', '0000000000', '20%麻痹', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('207', '闪电突袭', '电', '90', '1', '100', '00000000000', '0000000000', '先制+1，8%反弹', '先手', '反噬', null, null, null);
INSERT INTO `skill_book` VALUES ('209', '交流电', '电', '70', '3', '100', '00000000000', '0000000000', '攻击后换人', '交换巴迪', null, null, null, null);
INSERT INTO `skill_book` VALUES ('214', '能量冲击', '电', '120', '1', '100', '00000000000', '0000000000', '8%反伤，10%麻痹', '麻痹', '反噬', null, null, null);
INSERT INTO `skill_book` VALUES ('215', '充电', '电', '0', '2', '101', '00000000000', '0000000000', '', '强化特攻', '电系威力*1.5', null, null, null);
INSERT INTO `skill_book` VALUES ('218', '十万伏特', '电', '95', '3', '100', '00000000000', '0000000000', '20%麻痹', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('219', '电击', '电', '40', '3', '100', '00000000000', '0000000000', '30%麻痹', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('220', '雷光拳', '电', '80', '1', '100', '00000000000', '0000000000', '30%麻痹', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('223', '暴风', '飞', '110', '3', '70', '00000000000', '0000000000', '30%混乱', '混乱', null, null, null, null);
INSERT INTO `skill_book` VALUES ('224', '空气切割', '飞', '80', '3', '95', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('226', '勇鸟', '飞', '120', '1', '100', '00000000000', '0000000000', '8%反伤', '反噬', null, null, null, null);
INSERT INTO `skill_book` VALUES ('227', '羽栖', '飞', '0', '2', '101', '00000000000', '0000000000', '', '回复50%HP', null, null, null, null);
INSERT INTO `skill_book` VALUES ('229', '燕反', '飞', '60', '1', '101', '00000000000', '0000000000', '', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('230', '风之伤', '飞', '120', '1', '100', '00000000000', '0000000000', '连续攻击2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('231', '空气爆炸', '飞', '100', '3', '90', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('232', '神鸟', '飞', '150', '1', '100', '00000000000', '0000000000', '先制-3，出招前受到攻击则失败', '后手', '蓄力', null, null, null);
INSERT INTO `skill_book` VALUES ('233', '钻孔啄', '飞', '80', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('234', '飞空', '飞', '100', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('237', '叶刃斩', '草', '90', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('239', '木角', '草', '75', '1', '100', '00000000000', '0000000000', '50%伤害回复自身', '吸血', null, null, null, null);
INSERT INTO `skill_book` VALUES ('240', '种子闪光弹', '草', '120', '3', '80', '00000000000', '0000000000', '20%特防-2', '弱化特防', '弱化特防', null, null, null);
INSERT INTO `skill_book` VALUES ('241', '木槌', '草', '120', '1', '100', '00000000000', '0000000000', '8%反伤', '反噬', null, null, null, null);
INSERT INTO `skill_book` VALUES ('242', '叶风暴', '草', '140', '3', '90', '00000000000', '0000000000', '特攻-2', '对自己发动特效', '弱化特攻', '弱化特攻', null, null);
INSERT INTO `skill_book` VALUES ('243', '能源球', '草', '90', '3', '100', '00000000000', '0000000000', '20%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('244', '种子爆弹', '草', '80', '1', '100', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('247', '草笛', '草', '0', '2', '55', '00000000000', '0000000000', '', '催眠', null, null, null, null);
INSERT INTO `skill_book` VALUES ('248', '芳香疗法', '草', '0', '2', '101', '00000000000', '0000000000', '', '全体异常解除', null, null, null, null);
INSERT INTO `skill_book` VALUES ('250', '光合作用', '草', '0', '2', '101', '00000000000', '0000000000', '', '回复50%HP,晴天2倍，其它天气减半', null, null, null, null);
INSERT INTO `skill_book` VALUES ('252', '花瓣舞', '草', '120', '3', '100', '00000000000', '0000000000', '连续使用2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('253', '睡眠粉', '草', '0', '2', '75', '00000000000', '0000000000', '', '催眠', null, null, null, null);
INSERT INTO `skill_book` VALUES ('254', '麻痹粉', '草', '0', '2', '75', '00000000000', '0000000000', '', '麻痹', null, null, null, null);
INSERT INTO `skill_book` VALUES ('255', '阳光烈焰', '草', '120', '3', '100', '00000000000', '0000000000', '蓄力1回合，第二回攻击，晴天不用蓄力', '太阳能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('257', '寄生种子', '草', '0', '2', '85', '00000000000', '0000000000', '', '每回合吸收1/8体力', null, null, null, null);
INSERT INTO `skill_book` VALUES ('259', '飞叶快刀', '草', '40', '1', '100', '00000000000', '0000000000', '', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('260', '飘花淡雪浮香吹', '草', '90', '3', '100', '00000000000', '0000000000', 'CT+1,10%冰封，退场', '后手', '暴击', '冰冻', '强制退场', null);
INSERT INTO `skill_book` VALUES ('263', '影袭', '鬼', '40', '3', '100', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('264', '鬼爪', '鬼', '80', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('265', '鬼影', '鬼', '80', '1', '100', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('267', '影子球', '鬼', '90', '3', '100', '00000000000', '0000000000', '30%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('268', '同归', '鬼', '0', '2', '101', '00000000000', '0000000000', '', '拉对手一起死', null, null, null, null);
INSERT INTO `skill_book` VALUES ('269', '奇怪光', '鬼', '0', '2', '100', '00000000000', '0000000000', '', '混乱', null, null, null, null);
INSERT INTO `skill_book` VALUES ('270', '百鬼夜行', '鬼', '135', '3', '85', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('271', '异次元冲锋', '恶', '120', '3', '100', '00000000000', '0000000000', '连续攻击2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('273', '黑夜爆裂', '恶', '75', '3', '95', '00000000000', '0000000000', 'CT+2', '暴击', '暴击', null, null, null);
INSERT INTO `skill_book` VALUES ('274', '欺诈', '恶', '95', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('275', '磨爪子', '恶', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', '提高暴击率', null, null, null);
INSERT INTO `skill_book` VALUES ('276', '黑洞', '恶', '0', '2', '80', '00000000000', '0000000000', '', '催眠', null, null, null, null);
INSERT INTO `skill_book` VALUES ('277', '诡计', '恶', '0', '2', '101', '00000000000', '0000000000', '', '强化特攻', '强化特攻', null, null, null);
INSERT INTO `skill_book` VALUES ('278', '掉包', '恶', '0', '2', '100', '00000000000', '0000000000', '', '交换物品', null, null, null, null);
INSERT INTO `skill_book` VALUES ('279', '试刀', '恶', '85', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('280', '恶念波动', '恶', '80', '3', '100', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('281', '偷袭', '恶', '60', '1', '100', '00000000000', '0000000000', '对方攻击时，先制+3，否则失败', '100%发动特效', '暴击', null, null, null);
INSERT INTO `skill_book` VALUES ('318', '补刀', '恶', '60', '1', '100', '00000000000', '0000000000', '击倒对手时强化物攻', '击倒对手时候发动特效', '强化物攻', null, null, null);
INSERT INTO `skill_book` VALUES ('319', '落井下石', '恶', '60', '1', '100', '00000000000', '0000000000', '本回合内如果对手已经受到伤害，威力翻倍', '对方已受伤害时发动特效', '威力翻倍', null, null, null);
INSERT INTO `skill_book` VALUES ('320', '暗算', '恶', '60', '1', '101', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('321', '啃咬', '恶', '75', '1', '100', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('322', '咬碎', '恶', '90', '1', '100', '00000000000', '0000000000', '30%物防-1', '弱化物防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('323', '寻衅', '恶', '0', '2', '100', '00000000000', '0000000000', '', '不能连续使用相同技能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('326', '挑拨', '恶', '0', '2', '100', '00000000000', '0000000000', '', '3回合不能使用变化技能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('327', '精神切割', '超', '80', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('328', '心灵压迫', '超', '80', '1', '90', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('330', '念力', '超', '50', '3', '100', '00000000000', '0000000000', '30%混乱', '混乱', null, null, null, null);
INSERT INTO `skill_book` VALUES ('334', '精神干扰', '超', '100', '3', '100', '00000000000', '0000000000', '30%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('335', '精神冲击', '超', '100', '3', '100', '00000000000', '0000000000', '物理伤害', '物理伤害', null, null, null, null);
INSERT INTO `skill_book` VALUES ('336', '精神病', '超', '120', '3', '100', '00000000000', '0000000000', '连续攻击2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('337', '预知未来', '超', '120', '3', '100', '00000000000', '0000000000', '2回合后攻击出现', '攻击延后2回合', null, null, null, null);
INSERT INTO `skill_book` VALUES ('338', '精神增压', '超', '140', '3', '90', '00000000000', '0000000000', '特攻-2', '对自己发动特效', '弱化特攻', '弱化特攻', null, null);
INSERT INTO `skill_book` VALUES ('339', '催眠术', '超', '0', '2', '60', '00000000000', '0000000000', '', '催眠', null, null, null, null);
INSERT INTO `skill_book` VALUES ('340', '高速移动', '超', '0', '2', '101', '00000000000', '0000000000', '', '强化速度', '强化速度', null, null, null);
INSERT INTO `skill_book` VALUES ('341', '栅栏', '超', '0', '2', '101', '00000000000', '0000000000', '', '强化物防', '强化物防', null, null, null);
INSERT INTO `skill_book` VALUES ('342', '光之壁', '超', '0', '2', '101', '00000000000', '0000000000', '', '5回合特殊伤害减半', null, null, null, null);
INSERT INTO `skill_book` VALUES ('343', '反射盾', '超', '0', '2', '101', '00000000000', '0000000000', '', '5回合物理伤害减半', null, null, null, null);
INSERT INTO `skill_book` VALUES ('344', '超级健忘', '超', '0', '2', '101', '00000000000', '0000000000', '', '强化特防', '强化特防', null, null, null);
INSERT INTO `skill_book` VALUES ('345', '封印', '超', '0', '2', '101', '00000000000', '0000000000', '', '对方不能使用我方会的技能', null, null, null, null);
INSERT INTO `skill_book` VALUES ('346', '冥想', '超', '0', '2', '101', '00000000000', '0000000000', '', '强化特攻', '强化特防', null, null, null);
INSERT INTO `skill_book` VALUES ('347', '重力', '超', '0', '2', '101', '00000000000', '0000000000', '', '5回合浮游与飞行系能被地系技能打中', null, null, null, null);
INSERT INTO `skill_book` VALUES ('348', '欺骗空间', '超', '0', '2', '101', '00000000000', '0000000000', '', '5回合速度慢的先行动', null, null, null, null);
INSERT INTO `skill_book` VALUES ('349', '龙之尾', '龙', '80', '1', '90', '00000000000', '0000000000', '强制换人，对野生怪兽则结束战斗。', '乱舞', '强制退场', null, null, null);
INSERT INTO `skill_book` VALUES ('350', '龙之爪', '龙', '90', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('351', '龙之冲锋', '龙', '100', '1', '85', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('352', '逆鳞', '龙', '120', '1', '100', '00000000000', '0000000000', '连续攻击2～3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('354', '龙之吐息', '龙', '90', '3', '90', '00000000000', '0000000000', '30%害怕。', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('356', '亚空切断', '龙', '100', '3', '90', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('357', '龙星群', '龙', '140', '3', '90', '00000000000', '0000000000', '特攻-2', '对自己发动特效', '弱化特攻', '弱化特攻', null, null);
INSERT INTO `skill_book` VALUES ('359', '龙之舞', '龙', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', '强化速度', null, null, null);
INSERT INTO `skill_book` VALUES ('360', '螺旋球', '钢', '90', '1', '100', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('361', '子弹拳', '钢', '40', '1', '100', '00000000000', '0000000000', '先制+1', '先手', null, null, null, null);
INSERT INTO `skill_book` VALUES ('362', '钢爪', '钢', '60', '1', '95', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('365', '铁头槌', '钢', '80', '1', '100', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('367', '彗星拳', '钢', '100', '1', '90', '00000000000', '0000000000', '20%物攻+1', '强化物攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('368', '镜面射击', '钢', '110', '3', '90', '00000000000', '0000000000', '30%特攻+1', '强化特攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('369', '光栅加农', '钢', '120', '3', '85', '00000000000', '0000000000', '30%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('370', '破灭之愿', '钢', '130', '3', '100', '00000000000', '0000000000', '2回合后，攻击出现', '攻击延后2回合', null, null, null, null);
INSERT INTO `skill_book` VALUES ('371', '变换齿轮', '钢', '0', '2', '101', '00000000000', '0000000000', '', '强化速度', '强化速度', null, null, null);
INSERT INTO `skill_book` VALUES ('372', '铁壁', '钢', '0', '2', '101', '00000000000', '0000000000', '', '强化物防', '强化物防', null, null, null);
INSERT INTO `skill_book` VALUES ('494', '终极针刺', '虫', '140', '1', '90', '00000000000', '0000000000', '', '下回合不能行动', null, null, null, null);
INSERT INTO `skill_book` VALUES ('496', '蜻蜓回转', '虫', '70', '1', '100', '00000000000', '0000000000', '攻击后交换怪兽出场', '交换巴迪', null, null, null, null);
INSERT INTO `skill_book` VALUES ('497', '十字剪', '虫', '90', '1', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('499', '百万角击', '虫', '120', '1', '85', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('500', '蜂涌', '虫', '120', '1', '100', '00000000000', '0000000000', '连续攻击2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('501', '信号光线', '虫', '75', '3', '100', '00000000000', '0000000000', '30%特攻+1', '强化特攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('502', '虫鸣', '虫', '90', '3', '100', '00000000000', '0000000000', '20%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('503', '蜘蛛网', '虫', '0', '2', '100', '00000000000', '0000000000', '', '禁止换人和逃跑', null, null, null, null);
INSERT INTO `skill_book` VALUES ('504', '萤火', '虫', '0', '2', '101', '00000000000', '0000000000', '', '强化特攻', '强化特攻', null, null, null);
INSERT INTO `skill_book` VALUES ('505', '蝶之舞', '虫', '0', '2', '101', '00000000000', '0000000000', '', '强化特攻', '强化特防', '强化速度', null, null);
INSERT INTO `skill_book` VALUES ('506', '丢石头', '岩', '30', '1', '90', '00000000000', '0000000000', '', null, null, null, null, null);
INSERT INTO `skill_book` VALUES ('508', '岩崩', '岩', '85', '1', '95', '00000000000', '0000000000', '30%害怕', '眩晕', null, null, null, null);
INSERT INTO `skill_book` VALUES ('509', '石刃', '岩', '100', '1', '80', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('510', '钻石风暴', '岩', '120', '1', '90', '00000000000', '0000000000', '连续使用2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('511', '岩石炮', '岩', '150', '1', '90', '00000000000', '0000000000', '下回合不能行动', '下回合不能行动', null, null, null, null);
INSERT INTO `skill_book` VALUES ('512', '双刃头槌', '岩', '150', '1', '80', '00000000000', '0000000000', '8%反伤', '反噬', null, null, null, null);
INSERT INTO `skill_book` VALUES ('513', '原始力量', '岩', '90', '3', '100', '00000000000', '0000000000', 'CT+1', '暴击', null, null, null, null);
INSERT INTO `skill_book` VALUES ('514', '力量宝石', '岩', '80', '3', '100', '00000000000', '0000000000', '30%特攻+1', '强化特攻', null, null, null, null);
INSERT INTO `skill_book` VALUES ('515', '沙暴', '岩', '0', '2', '101', '00000000000', '0000000000', '', '5回合沙暴天气，地岩钢以外属性巴迪受到1/16伤害', null, null, null, null);
INSERT INTO `skill_book` VALUES ('516', '岩切', '岩', '0', '2', '101', '00000000000', '0000000000', '', '强化速度', '强化速度', null, null, null);
INSERT INTO `skill_book` VALUES ('518', '地裂', '地', '0', '1', '30', '00000000000', '0000000000', '一击毙', '1击毙', null, null, null, null);
INSERT INTO `skill_book` VALUES ('519', '万箭齐发', '地', '90', '1', '100', '00000000000', '0000000000', '可击中飞行系', '无盲点', null, null, null, null);
INSERT INTO `skill_book` VALUES ('520', '地震', '地', '100', '1', '100', '00000000000', '0000000000', '全体攻击', 'AOE', null, null, null, null);
INSERT INTO `skill_book` VALUES ('521', '坠崖', '地', '120', '1', '100', '00000000000', '0000000000', '连续攻击2~3回合', '乱舞', null, null, null, null);
INSERT INTO `skill_book` VALUES ('522', '大地之力', '地', '90', '3', '100', '00000000000', '0000000000', '20%特防-1', '弱化特防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('523', '撒菱', '地', '0', '2', '101', '00000000000', '0000000000', '', '使上场巴迪受到1/8伤害', null, null, null, null);
INSERT INTO `skill_book` VALUES ('524', '剧毒之牙', '毒', '80', '1', '100', '00000000000', '0000000000', '30%中毒', '中毒', null, null, null, null);
INSERT INTO `skill_book` VALUES ('526', '毒突', '毒', '80', '1', '100', '00000000000', '0000000000', '30%物防-1', '弱化物防', null, null, null, null);
INSERT INTO `skill_book` VALUES ('527', '粉尘射击', '毒', '120', '1', '80', '00000000000', '0000000000', '10%中毒', '中毒', null, null, null, null);
INSERT INTO `skill_book` VALUES ('528', '毒液冲击', '毒', '60', '3', '100', '00000000000', '0000000000', '毒发', '对方中毒时发动特效', '威力翻倍', null, null, null);
INSERT INTO `skill_book` VALUES ('529', '淤泥爆弹', '毒', '90', '3', '100', '00000000000', '0000000000', '20%中毒。', '中毒', null, null, null, null);
INSERT INTO `skill_book` VALUES ('530', '毒粉', '毒', '0', '2', '75', '00000000000', '0000000000', '', '中毒', null, null, null, null);
INSERT INTO `skill_book` VALUES ('531', '剧毒', '毒', '0', '2', '85', '00000000000', '0000000000', '', '中毒', null, null, null, null);
INSERT INTO `skill_book` VALUES ('533', '盘蜷', '毒', '0', '2', '101', '00000000000', '0000000000', '', '强化物攻', '强化物防', null, null, null);
INSERT INTO `skill_book` VALUES ('534', '香甜气息', '草', '0', '2', '100', '00000000001', '0000000000', '', '催眠', '非战斗使用可引出野生巴迪', null, null, null);

-- ----------------------------
-- Table structure for `talent`
-- ----------------------------
DROP TABLE IF EXISTS `talent`;
CREATE TABLE `talent` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL COMMENT '特性名',
  `descript` varchar(255) DEFAULT NULL COMMENT '特性描述',
  `effect` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `name_2` (`name`),
  KEY `name_3` (`name`),
  KEY `effect_name21` (`effect`),
  CONSTRAINT `effect_name21` FOREIGN KEY (`effect`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=104 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of talent
-- ----------------------------
INSERT INTO `talent` VALUES ('1', '威吓', '对放物理攻击降低1级', '弱化物攻');
INSERT INTO `talent` VALUES ('2', '石脑', '不受反弹伤害', '不受反噬');
INSERT INTO `talent` VALUES ('3', '净体', '不被弱化', '不被弱化');
INSERT INTO `talent` VALUES ('4', '压力', '对方容易失误', '提高闪避');
INSERT INTO `talent` VALUES ('5', '恶臭', '10%害怕', null);
INSERT INTO `talent` VALUES ('6', '暴雨', null, null);
INSERT INTO `talent` VALUES ('7', '扬沙', null, null);
INSERT INTO `talent` VALUES ('8', '干旱', null, null);
INSERT INTO `talent` VALUES ('9', '甲虫盔甲', null, null);
INSERT INTO `talent` VALUES ('10', '结识', '不受1击毙，忍耐', null);
INSERT INTO `talent` VALUES ('11', '湿气', '不能爆炸，引爆', null);
INSERT INTO `talent` VALUES ('12', '柔软', '不被麻痹', null);
INSERT INTO `talent` VALUES ('13', '沙隐', '沙暴闪避提升', null);
INSERT INTO `talent` VALUES ('14', '雪隐', '冰雹闪避提升', null);
INSERT INTO `talent` VALUES ('15', '静电', '受到物理攻击，麻痹对手', null);
INSERT INTO `talent` VALUES ('16', '食草', '草系无效，强化物攻', null);
INSERT INTO `talent` VALUES ('17', '蓄水', '水系无效，回复1/4体力', null);
INSERT INTO `talent` VALUES ('18', '引火', '火系无效，火系威力*1.5', null);
INSERT INTO `talent` VALUES ('19', '电气引擎', '电系无效，强化速度', null);
INSERT INTO `talent` VALUES ('20', '无天气', null, null);
INSERT INTO `talent` VALUES ('21', '复眼', null, null);
INSERT INTO `talent` VALUES ('22', '失眠', null, null);
INSERT INTO `talent` VALUES ('23', '免疫', null, null);
INSERT INTO `talent` VALUES ('24', '变色', '变成打击属性', null);
INSERT INTO `talent` VALUES ('25', '磷粉', '不吃特效', null);
INSERT INTO `talent` VALUES ('26', '我行我素', '不混乱', null);
INSERT INTO `talent` VALUES ('27', '吸盘', '不退场', null);
INSERT INTO `talent` VALUES ('28', '踩影', '禁止逃跑，换人', null);
INSERT INTO `talent` VALUES ('29', '沙地狱', '禁止逃跑，换人', null);
INSERT INTO `talent` VALUES ('30', '蛇皮', '物理攻击，反弹', null);
INSERT INTO `talent` VALUES ('31', '怪力', '攻击不减', null);
INSERT INTO `talent` VALUES ('32', '懒惰', '2回合行动1次', null);
INSERT INTO `talent` VALUES ('33', '活力', '升攻击*1.5，命中*0.8', null);
INSERT INTO `talent` VALUES ('34', '黏着', '物品不会丢失', null);
INSERT INTO `talent` VALUES ('35', '蜕皮', '30%接触异常', null);
INSERT INTO `talent` VALUES ('36', '根性', '异常时物攻*1.5，烧伤不减物攻', null);
INSERT INTO `talent` VALUES ('37', '神秘鳞片', '异常时物防*1.5', null);
INSERT INTO `talent` VALUES ('38', '毒液', '被吸血时对方掉血', null);
INSERT INTO `talent` VALUES ('39', '茂盛', '体力1/3时草系威力*1.5', null);
INSERT INTO `talent` VALUES ('40', '猛火', '体力1/3时火系威力*1.5', null);
INSERT INTO `talent` VALUES ('41', '激流', '体力1/3时水系威力*1.5', null);
INSERT INTO `talent` VALUES ('42', '虫族报警', '体力1/3时虫系威力*1.5', null);
INSERT INTO `talent` VALUES ('43', '瑜伽之力', '物攻*2', null);
INSERT INTO `talent` VALUES ('44', '浮游', '不受地属性攻击', null);
INSERT INTO `talent` VALUES ('45', '孢子', '物理攻击10%麻痹，中毒，催眠', null);
INSERT INTO `talent` VALUES ('46', '自然恢复', '退场解除异常', null);
INSERT INTO `talent` VALUES ('47', '天恩', '特效几率*2', null);
INSERT INTO `talent` VALUES ('48', '轻快', '雨天速度*2', null);
INSERT INTO `talent` VALUES ('49', '叶绿素', '晴天速度*2', null);
INSERT INTO `talent` VALUES ('50', '挖沙', '沙暴速度*2，不受沙暴伤害', null);
INSERT INTO `talent` VALUES ('51', '换装', '复制特性', null);
INSERT INTO `talent` VALUES ('52', '精神力', '不眩晕', null);
INSERT INTO `talent` VALUES ('53', '熔岩盔甲', '不冰冻', null);
INSERT INTO `talent` VALUES ('54', '水幕', '不烧伤', null);
INSERT INTO `talent` VALUES ('55', '磁力', '钢属性不能交换逃跑', null);
INSERT INTO `talent` VALUES ('56', '雨盘', '雨天回复1/16', null);
INSERT INTO `talent` VALUES ('57', '加速', '每回合结束强化速度', null);
INSERT INTO `talent` VALUES ('58', '肥脂肪', '冰火属性伤害减半', null);
INSERT INTO `talent` VALUES ('59', '早起', '只睡1回合', null);
INSERT INTO `talent` VALUES ('60', '火焰体', '物理攻击30%烧伤', null);
INSERT INTO `talent` VALUES ('61', '逃走', '必定可以交换和逃跑', null);
INSERT INTO `talent` VALUES ('62', '锐利目光', '技能必中', null);
INSERT INTO `talent` VALUES ('63', '贝壳盔甲', '不受暴击', null);
INSERT INTO `talent` VALUES ('64', '不屈', '眩晕后强化速度', null);
INSERT INTO `talent` VALUES ('65', '正义心', '被恶属性攻击，强化物攻', null);
INSERT INTO `talent` VALUES ('66', '愤怒穴道', '被暴击物攻极限', null);
INSERT INTO `talent` VALUES ('67', '杂技', '无道具时速度*2', null);
INSERT INTO `talent` VALUES ('68', '耐热', '火属性伤害减半', null);
INSERT INTO `talent` VALUES ('69', '单纯', '强化效果翻倍', null);
INSERT INTO `talent` VALUES ('70', '铁拳', '拳类攻击伤害*1.2', null);
INSERT INTO `talent` VALUES ('71', '毒疗', '中毒时回合结束恢复1/8', null);
INSERT INTO `talent` VALUES ('72', '适应力', '本属性伤害*1.5', null);
INSERT INTO `talent` VALUES ('73', '湿润身躯', '雨天不异常，解除异常', null);
INSERT INTO `talent` VALUES ('74', '太阳力量', '晴天特攻*1.5，每回合结束损失1/8体力', null);
INSERT INTO `talent` VALUES ('75', '飞毛腿', '异常时候速度*1.5，麻痹不减速度', null);
INSERT INTO `talent` VALUES ('76', '强运', '提升暴击几率', null);
INSERT INTO `talent` VALUES ('77', '魔法防御', '只收攻击技能伤害', null);
INSERT INTO `talent` VALUES ('78', '无防御', '双方技能必中', null);
INSERT INTO `talent` VALUES ('79', '慢出', '后手', null);
INSERT INTO `talent` VALUES ('80', '技师', '威力小于60的技能伤害*1.5', null);
INSERT INTO `talent` VALUES ('81', '叶片防御', '晴天不异常，解除异常', null);
INSERT INTO `talent` VALUES ('82', '革新', '无视对方特性', null);
INSERT INTO `talent` VALUES ('83', '有色眼镜', '伤害无修正时，威力翻倍', null);
INSERT INTO `talent` VALUES ('84', '胆量', '可以击中鬼属性巴迪', null);
INSERT INTO `talent` VALUES ('85', '冰冻身体', '冰雹天每回合回复1/16', null);
INSERT INTO `talent` VALUES ('86', '舍身', '反噬类技能威力*1.2', null);
INSERT INTO `talent` VALUES ('87', '花之礼', '晴天强化特攻强化特防', null);
INSERT INTO `talent` VALUES ('88', '给力', '伤害*1.3，进攻特效不发动', null);
INSERT INTO `talent` VALUES ('89', '圣斗士', '见过的技能无效', null);
INSERT INTO `talent` VALUES ('90', '破甲', '弱化防御，强化速度', null);
INSERT INTO `talent` VALUES ('92', '防尘', '不受天气伤害', null);
INSERT INTO `talent` VALUES ('93', '毒手', '物理攻击30%使中毒', null);
INSERT INTO `talent` VALUES ('94', '再生', '退场回复1/3体力', null);
INSERT INTO `talent` VALUES ('95', '鸽胸', '物防不被弱化', null);
INSERT INTO `talent` VALUES ('96', '分析', '后手威力翻倍', null);
INSERT INTO `talent` VALUES ('97', '幻觉', '变成最后1只巴迪，受到攻击变回', null);
INSERT INTO `talent` VALUES ('98', '变身', '变成对方', null);
INSERT INTO `talent` VALUES ('99', '穿透', '无视光墙，反射盾', null);
INSERT INTO `talent` VALUES ('100', '自信过程', '击倒对手时强化物攻', null);
INSERT INTO `talent` VALUES ('101', '魔装反射', '反弹对方变化技能', null);
INSERT INTO `talent` VALUES ('102', '沙之力', '沙暴中地岩钢技能威力*1.5', null);
INSERT INTO `talent` VALUES ('103', '幸运之星', '己方暴击提升10%', null);
