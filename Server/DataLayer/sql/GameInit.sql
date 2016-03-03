/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50150
Source Host           : localhost:3306
Source Database       : gameinit

Target Server Type    : MYSQL
Target Server Version : 50150
File Encoding         : 65001

Date: 2016-02-16 18:56:42
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `battle_step`
-- ----------------------------
DROP TABLE IF EXISTS `battle_step`;
CREATE TABLE `battle_step` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '时机id',
  `step` varchar(255) DEFAULT NULL COMMENT '时机描述',
  PRIMARY KEY (`id`),
  KEY `step` (`step`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of battle_step
-- ----------------------------
INSERT INTO `battle_step` VALUES ('8', '非战斗');
INSERT INTO `battle_step` VALUES ('2', '换人');
INSERT INTO `battle_step` VALUES ('6', '回合结束');
INSERT INTO `battle_step` VALUES ('1', '回合开始');
INSERT INTO `battle_step` VALUES ('4', '使用物品');
INSERT INTO `battle_step` VALUES ('5', '战斗');
INSERT INTO `battle_step` VALUES ('3', '准备战斗');

-- ----------------------------
-- Table structure for `buddy_book_info`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_book_info`;
CREATE TABLE `buddy_book_info` (
  `number` int(11) NOT NULL COMMENT '全国编号',
  `name` varchar(255) NOT NULL COMMENT '名字',
  `descript` varchar(60) DEFAULT NULL COMMENT '描述,最大60byte',
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
  `tame` int(11) NOT NULL DEFAULT '0' COMMENT '驯化率 1~99',
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
INSERT INTO `buddy_book_info` VALUES ('1', '怪蛙草', null, '草', '毒', '茂盛', null, '叶绿素', null, '80', '82', '83', '100', '100', '80', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('2', '爆炎龙', null, '火', '飞', '猛火', null, '太阳力量', null, '78', '84', '69', '109', '85', '100', '4', '100');
INSERT INTO `buddy_book_info` VALUES ('3', '巨炮龟', null, '水', null, '激流', null, '雨盘', null, '79', '83', '100', '85', '100', '78', '4', '100');
INSERT INTO `buddy_book_info` VALUES ('4', '幼虫', null, '虫', null, '逃走', '复眼', '磷粉', null, '45', '30', '35', '20', '20', '45', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('5', '雅美蝶', null, '虫', '飞', '复眼', '有色眼镜', '疾风之翼', null, '60', '45', '50', '90', '80', '70', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('6', '大仙子', null, '虫', '超', '自然恢复', '魔装反射', '花之礼', null, '105', '90', '70', '135', '90', '110', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('7', '兵蜂', '', '虫', '毒', '虫族报警', null, '强运', null, '65', '90', '40', '45', '80', '75', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('8', '蜂后', '', '虫', '斗', '霸气', '净体', '威吓', null, '90', '135', '80', '105', '70', '120', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('9', '彩雀', '', '飞', '普', '锐利目光', '逃走', '疾风之翼', null, '83', '80', '75', '70', '70', '101', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('10', '冰鸟', '', '冰', '飞', '冰冻身体', '圣斗士', '压力', null, '90', '85', '100', '95', '125', '85', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('11', '雷鸟', '', '电', '飞', '静电', '锐利目光', '压力', null, '90', '90', '85', '125', '90', '100', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('12', '凤凰', '', '火', '飞', '火焰体', '舍身', '圣斗士', null, '90', '100', '90', '125', '85', '90', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('13', '毛毛鼠', '', '普', null, '逃走', '根性', '胆量', null, '30', '56', '35', '25', '25', '72', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('14', '胖胖鼠', '', '普', null, '舍身', '根性', '胆量', null, '55', '81', '60', '50', '70', '97', '4', '0');
INSERT INTO `buddy_book_info` VALUES ('15', '穿山鼠', '', '地', null, '沙隐', null, '挖沙', null, '75', '100', '110', '45', '55', '65', '4', '0');
INSERT INTO `buddy_book_info` VALUES ('16', '飞天鼠', '', '飞', '毒', '穿透', '技师', '石脑', null, '85', '90', '80', '70', '80', '130', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('17', '芽角兔', null, '毒', null, '毒手', '毒疗', '强运', null, '46', '57', '40', '40', '40', '50', '4', '50');
INSERT INTO `buddy_book_info` VALUES ('18', '角兔怪', null, '毒', null, '毒手', '毒疗', '强运', null, '61', '72', '57', '55', '55', '65', '4', '0');
INSERT INTO `buddy_book_info` VALUES ('19', '暴龙兔', null, '毒', '地', '威吓', '霸气', '给力', null, '81', '102', '77', '85', '75', '85', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('20', '铁甲兔', null, '地', '岩', '石脑', '舍身', '锐利目光', null, '105', '130', '120', '45', '45', '40', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('21', '球狸', null, '普', null, '魔法防御', '食草', '天然', null, '70', '45', '48', '60', '65', '35', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('22', '胖狸', null, '普', null, '魔法防御', '食草', '天然', null, '95', '70', '73', '95', '90', '60', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('23', '炽天女', null, '火', '飞', '魔法防御', '正义心', '威吓', null, '135', '80', '80', '110', '105', '90', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('24', '小火狐', null, '火', null, '引火', '猛火', '干旱', null, '38', '41', '40', '50', '65', '65', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('25', '火狐', null, '火', null, '引火', '猛火', '干旱', null, '73', '76', '75', '81', '100', '100', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('26', '妲己', null, '火', '超', '引火', '猛火', '干旱', null, '95', '70', '80', '100', '135', '120', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('27', '大红花', null, '草', '毒', '叶绿素', '孢子', '磷粉', null, '75', '80', '85', '110', '90', '50', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('28', '红魔女', null, '草', '恶', '花之礼', '磷粉', '叶绿素', null, '95', '90', '95', '130', '110', '80', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('29', '土豆兄弟', null, '地', null, '咬脚', '沙隐', '沙之力', null, '35', '80', '50', '50', '70', '120', '4', '100');
INSERT INTO `buddy_book_info` VALUES ('30', '调皮猫', null, '普', null, '技师', '柔软', '给力', null, '40', '45', '30', '40', '40', '90', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('31', '邪心狮', null, '普', null, '技师', '柔软', '给力', null, '65', '70', '60', '65', '65', '115', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('32', '妖魅', null, '普', '恶', '技师', '柔软', '压力', null, '80', '105', '80', '100', '85', '150', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('33', '猪鼻猴', null, '斗', null, '失眠', '正义心', '锐利目光', null, '65', '105', '60', '60', '70', '95', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('34', '金毛吼', null, '火', null, '威吓', '引火', '正义心', null, '90', '110', '80', '100', '80', '95', '3', '80');
INSERT INTO `buddy_book_info` VALUES ('35', '勺子狸', null, '超', null, '精神力', '换装', '魔法防御', null, '55', '50', '45', '135', '95', '120', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('36', '拳鸡', null, '斗', null, '根性', '锐利目光', '不屈', null, '90', '130', '80', '65', '85', '55', '4', '80');
INSERT INTO `buddy_book_info` VALUES ('37', '外星水母', null, '水', '毒', '净体', null, '雨盘', null, '80', '70', '65', '80', '120', '100', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('38', '岩龟', null, '岩', '地', '石脑', '结实', '沙隐', null, '80', '120', '130', '55', '65', '45', '4', '80');
INSERT INTO `buddy_book_info` VALUES ('39', '赤足马', null, '火', null, '引火', '逃走', '火焰体', null, '50', '85', '55', '65', '65', '90', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('40', '赤炎兽', null, '火', null, '引火', '逃走', '火焰体', null, '65', '100', '70', '80', '80', '105', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('41', '三头鸵鸟', null, '飞', '普', '失眠', null, '舍身', null, '60', '110', '70', '60', '60', '100', '3', '70');
INSERT INTO `buddy_book_info` VALUES ('42', '角海狮', null, '水', '冰', '湿润身躯', '肥脂肪', '冰冻身体', null, '90', '70', '80', '70', '95', '70', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('43', '战船贝', null, '冰', '钢', '盔甲', '浮游', '冰冻皮肤', null, '85', '90', '180', '80', '40', '60', '4', '0');
INSERT INTO `buddy_book_info` VALUES ('44', '雾鬼', null, '鬼', '毒', '浮游', null, null, null, '40', '55', '50', '100', '55', '80', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('45', '霾鬼', null, '鬼', '毒', '浮游', null, null, null, '60', '65', '60', '130', '75', '110', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('46', '疼蛋', null, '草', '超', '叶绿素', null, null, null, '60', '40', '80', '60', '45', '40', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('47', '椰蛋怪', null, '草', '超', '叶绿素', null, null, null, '105', '90', '90', '130', '60', '50', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('48', '土地鬼', null, '地', '鬼', '石脑', '盔甲', '暴力', null, '70', '100', '110', '50', '50', '45', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('49', '雾霾', null, '毒', null, '浮游', null, null, null, '80', '90', '130', '60', '70', '60', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('50', '幼海马', null, '水', null, '轻快', '强运', null, null, '30', '40', '70', '70', '25', '60', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('51', '海龙王', null, '水', '龙', '轻快', '强运', null, null, '75', '95', '95', '95', '95', '85', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('52', '剑鱼', null, '水', null, '轻快', '湿润身躯', '电气引擎', null, '80', '92', '65', '65', '80', '68', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('53', '美人鱼', null, '水', '斗', '轻快', '暴雨', '电气引擎', null, '120', '115', '80', '120', '80', '85', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('54', '飞刀螳', null, '虫', '飞', '技师', '虫族报警', '超级炮台', null, '80', '130', '80', '20', '70', '120', '2', '30');
INSERT INTO `buddy_book_info` VALUES ('55', '机械螳螂', null, '虫', '钢', '技师', '虫族报警', '暴力', null, '70', '130', '100', '55', '60', '65', '3', '40');
INSERT INTO `buddy_book_info` VALUES ('56', '老牛', null, '普', null, '威吓', '超级炮台', '胆量', null, '75', '100', '95', '40', '70', '110', '3', '50');
INSERT INTO `buddy_book_info` VALUES ('57', '鲤鱼', null, '水', null, '轻快', null, '胆量', null, '20', '10', '55', '15', '20', '80', '3', '60');
INSERT INTO `buddy_book_info` VALUES ('58', '鲤鱼龙', null, '水', '飞', '威吓', null, '轻快', null, '100', '130', '99', '40', '90', '81', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('59', '美女龙', null, '水', null, '神秘鳞片', null, '电气引擎', null, '100', '30', '99', '100', '130', '81', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('60', '棉棉龟', null, '水', '冰', '蓄水', '盔甲', '湿润身躯', null, '130', '85', '80', '85', '95', '60', '3', '60');
INSERT INTO `buddy_book_info` VALUES ('61', '女布', null, '普', null, '逃走', '逃走', '自然恢复', null, '55', '55', '50', '45', '65', '55', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('62', '雷布', null, '电', null, '电气引擎', null, '飞毛腿', null, '65', '65', '60', '110', '95', '130', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('63', '日布', null, '超', null, '换装', '超级炮台', '魔装反射', null, '75', '40', '60', '130', '110', '110', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('64', '月布', null, '恶', null, '换装', '精神力', '魔装反射', null, '95', '65', '110', '60', '130', '65', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('65', '破嘴螺', null, '水', '岩', '轻快', '盔甲', null, null, '70', '60', '125', '115', '70', '55', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('66', '水螳螂', null, '水', '岩', '轻快', '盔甲', null, null, '60', '115', '105', '65', '70', '80', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('67', '风速龙', null, '飞', '岩', '压力', '石脑', '霸气', null, '80', '105', '65', '60', '75', '130', '3', '60');
INSERT INTO `buddy_book_info` VALUES ('68', '冰鳅', null, '龙', null, '蜕皮', null, '神秘鳞片', null, '41', '64', '45', '50', '50', '50', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('69', '冰龙', null, '龙', null, '蜕皮', null, '神秘鳞片', null, '61', '84', '65', '70', '70', '70', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('70', '雪女', null, '冰', '龙', '暴雪', null, '冰冻皮肤', null, '91', '134', '95', '100', '100', '80', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('71', '魔鬼鱼', null, '电', '水', '电气引擎', null, '蓄水', null, '125', '38', '38', '96', '76', '87', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('72', '星蛋', null, '普', null, '逃走', null, '强运', null, '35', '20', '65', '40', '65', '20', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('73', '星妞', null, '飞', '超', '强运', null, '天然', null, '100', '100', '100', '100', '100', '100', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('74', '玩偶羊', null, '电', null, '静电', null, '魔装反射', null, '90', '75', '85', '115', '90', '55', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('75', '轻轻花', null, '草', '飞', '叶绿素', '叶片防御', '穿透', null, '75', '55', '70', '55', '95', '110', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('76', '风精灵', null, '草', '飞', '叶片防御', '茂盛', '肥脂肪', null, '105', '35', '75', '100', '135', '150', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('77', '红蜻蜓', null, '虫', '飞', '有色眼镜', null, '加速', null, '86', '76', '86', '116', '56', '95', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('78', '怪娃鱼', null, '地', '水', '蓄水', '天然', '食草', null, '95', '85', '85', '65', '65', '35', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('79', '钢果兽', null, '虫', '钢', '盔甲', '结实', null, null, '75', '90', '140', '60', '60', '40', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('80', '铁蝎子', null, '钢', '地', '结实', '石脑', '给力', null, '75', '85', '200', '55', '65', '30', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('81', '象牙犬', null, '普', null, '威吓', null, '胆量', null, '105', '120', '100', '5', '80', '40', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('82', '怪力甲壳虫', null, '虫', '斗', '根性', '虫族报警', '胆量', null, '80', '130', '75', '40', '90', '85', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('83', '地狱犬', null, '火', '恶', '引火', '失眠', '猛火', null, '45', '60', '30', '80', '50', '65', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('84', '魔羊', null, '火', '恶', '引火', '失眠', '太阳力量', null, '75', '90', '50', '110', '80', '95', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('85', '大恶魔', null, '火', '恶', '霸气', '火焰体', '黑暗气息', null, '100', '135', '60', '135', '60', '110', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('86', '皮皮象', null, '地', null, '结实', null, '沙隐', null, '90', '120', '120', '60', '60', '50', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('87', '绿芽龙', null, '地', '岩', '根性', '沙隐', null, null, '50', '64', '50', '45', '50', '41', '3', '70');
INSERT INTO `buddy_book_info` VALUES ('88', '沙漠暴龙', null, '岩', '恶', '扬沙', null, null, null, '100', '134', '110', '95', '100', '61', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('89', '针叶蜥', null, '草', null, '茂盛', null, null, null, '70', '85', '65', '105', '85', '120', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('90', '超级鸟', null, '火', '斗', '猛火', '舍身', '加速', null, '80', '120', '70', '110', '70', '80', '3', '70');
INSERT INTO `buddy_book_info` VALUES ('91', '大水狗', null, '水', '地', '激流', null, null, null, '100', '110', '90', '85', '90', '60', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('92', '菠萝鸭', null, '水', '草', '轻快', '雨盘', null, null, '80', '70', '70', '90', '100', '70', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('93', '斗菇', null, '草', '斗', '毒疗', '孢子', '技师', null, '60', '130', '80', '60', '60', '70', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('94', '臭美猫', null, '普', null, '再生', '变身', '魔法防御', null, '70', '65', '65', '55', '55', '70', '3', '30');
INSERT INTO `buddy_book_info` VALUES ('95', '修罗女', null, '普', '恶', '柔软', '变身', '黑暗气息', null, '90', '120', '65', '125', '65', '135', '1', '0');
INSERT INTO `buddy_book_info` VALUES ('96', '钻石鬼', null, '鬼', '恶', '锐利目光', '分析', '再生', null, '50', '75', '75', '65', '65', '50', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('97', '瑜伽猴', null, '斗', '超', '暴力', null, null, null, '60', '70', '70', '60', '70', '80', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('98', '贪吃鬼', null, '毒', null, '暴力', '强运', null, null, '130', '73', '130', '53', '53', '25', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('99', '虎鲨', null, '水', '恶', '蛇皮', '强运', '加速', null, '70', '120', '40', '95', '40', '95', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('100', '咬力蚁', null, '地', null, '咬脚', null, '给力', null, '45', '100', '45', '45', '45', '10', '3', '50');
INSERT INTO `buddy_book_info` VALUES ('101', '蜻蜓龙', null, '地', '龙', '浮游', null, '给力', null, '80', '100', '80', '80', '80', '100', '2', '0');
INSERT INTO `buddy_book_info` VALUES ('102', '云雀', null, '龙', '飞', '自然恢复', null, null, null, '90', '60', '100', '65', '105', '80', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('103', '暗杀兔', null, '普', null, '免疫', '根性', '超级炮台', null, '72', '120', '55', '55', '55', '101', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('104', '悬浮魔偶', null, '超', '地', '浮游', null, null, null, '60', '70', '105', '70', '120', '75', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('105', '夜魔人', null, '鬼', null, '压力', null, null, null, '40', '70', '130', '60', '130', '25', '5', '100');
INSERT INTO `buddy_book_info` VALUES ('106', '石脑龙', null, '龙', null, '石脑', null, '给力', null, '45', '75', '60', '40', '30', '50', '4', '70');
INSERT INTO `buddy_book_info` VALUES ('107', '飞天血蛤', null, '龙', '飞', '威吓', null, '天空皮肤', null, '95', '135', '80', '110', '80', '100', '3', '0');
INSERT INTO `buddy_book_info` VALUES ('108', '铁手臂', null, '钢', null, '净体', null, null, null, '40', '55', '80', '35', '60', '30', '4', '30');
INSERT INTO `buddy_book_info` VALUES ('109', '机器手', null, '钢', '超', '净体', null, null, null, '60', '75', '100', '55', '80', '50', '4', '0');
INSERT INTO `buddy_book_info` VALUES ('110', '钢螃蟹', null, '钢', '超', '净体', null, '天然', null, '80', '150', '130', '90', '80', '70', '3', '0');

-- ----------------------------
-- Table structure for `buddy_book_skill`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_book_skill`;
CREATE TABLE `buddy_book_skill` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `buddy` varchar(255) NOT NULL COMMENT '巴迪兽名字',
  `skill` varchar(255) NOT NULL COMMENT '技能',
  `isInit` int(10) unsigned zerofill DEFAULT '0000000000' COMMENT '是否初始技能\r\n1是\r\n0不是',
  PRIMARY KEY (`id`),
  KEY `skill_key` (`skill`),
  KEY `buddy_key` (`buddy`),
  CONSTRAINT `skill_key` FOREIGN KEY (`skill`) REFERENCES `skill_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `buddy_key` FOREIGN KEY (`buddy`) REFERENCES `buddy_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=gb2312 COMMENT='巴迪兽图鉴——技能表';

-- ----------------------------
-- Records of buddy_book_skill
-- ----------------------------
INSERT INTO `buddy_book_skill` VALUES ('1', '爆炎龙', '吞噬', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('2', '爆炎龙', '喷射火焰', '0000000000');
INSERT INTO `buddy_book_skill` VALUES ('3', '巨炮龟', '摘星术', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('4', '巨炮龟', '沸水', '0000000000');
INSERT INTO `buddy_book_skill` VALUES ('5', '怪蛙草', '香甜气息', '0000000001');
INSERT INTO `buddy_book_skill` VALUES ('6', '怪蛙草', '能源球', '0000000000');

-- ----------------------------
-- Table structure for `buddy_book_update`
-- ----------------------------
DROP TABLE IF EXISTS `buddy_book_update`;
CREATE TABLE `buddy_book_update` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `buddy` varchar(255) NOT NULL,
  `upBuddy` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `ub_k_1` (`upBuddy`),
  KEY `b_k_1` (`buddy`),
  CONSTRAINT `b_k_1` FOREIGN KEY (`buddy`) REFERENCES `buddy_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `ub_k_1` FOREIGN KEY (`upBuddy`) REFERENCES `buddy_book_info` (`name`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=44 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of buddy_book_update
-- ----------------------------
INSERT INTO `buddy_book_update` VALUES ('1', '幼虫', '兵蜂');
INSERT INTO `buddy_book_update` VALUES ('2', '幼虫', '雅美蝶');
INSERT INTO `buddy_book_update` VALUES ('3', '雅美蝶', '大仙子');
INSERT INTO `buddy_book_update` VALUES ('4', '兵蜂', '蜂后');
INSERT INTO `buddy_book_update` VALUES ('5', '彩雀', '冰鸟');
INSERT INTO `buddy_book_update` VALUES ('6', '彩雀', '雷鸟');
INSERT INTO `buddy_book_update` VALUES ('7', '彩雀', '凤凰');
INSERT INTO `buddy_book_update` VALUES ('8', '毛毛鼠', '胖胖鼠');
INSERT INTO `buddy_book_update` VALUES ('9', '毛毛鼠', '穿山鼠');
INSERT INTO `buddy_book_update` VALUES ('10', '毛毛鼠', '飞天鼠');
INSERT INTO `buddy_book_update` VALUES ('11', '芽角兔', '角兔怪');
INSERT INTO `buddy_book_update` VALUES ('12', '角兔怪', '暴龙兔');
INSERT INTO `buddy_book_update` VALUES ('13', '角兔怪', '铁甲兔');
INSERT INTO `buddy_book_update` VALUES ('14', '球狸', '胖狸');
INSERT INTO `buddy_book_update` VALUES ('15', '胖狸', '炽天女');
INSERT INTO `buddy_book_update` VALUES ('16', '小火狐', '火狐');
INSERT INTO `buddy_book_update` VALUES ('17', '火狐', '妲己');
INSERT INTO `buddy_book_update` VALUES ('18', '大红花', '红魔女');
INSERT INTO `buddy_book_update` VALUES ('19', '调皮猫', '邪心狮');
INSERT INTO `buddy_book_update` VALUES ('20', '邪心狮', '妖魅');
INSERT INTO `buddy_book_update` VALUES ('21', '赤足马', '赤炎兽');
INSERT INTO `buddy_book_update` VALUES ('22', '角海狮', '战船贝');
INSERT INTO `buddy_book_update` VALUES ('23', '雾鬼', '霾鬼');
INSERT INTO `buddy_book_update` VALUES ('24', '疼蛋', '椰蛋怪');
INSERT INTO `buddy_book_update` VALUES ('25', '幼海马', '海龙王');
INSERT INTO `buddy_book_update` VALUES ('26', '剑鱼', '美人鱼');
INSERT INTO `buddy_book_update` VALUES ('27', '鲤鱼', '鲤鱼龙');
INSERT INTO `buddy_book_update` VALUES ('28', '鲤鱼', '美女龙');
INSERT INTO `buddy_book_update` VALUES ('29', '女布', '雷布');
INSERT INTO `buddy_book_update` VALUES ('30', '女布', '日布');
INSERT INTO `buddy_book_update` VALUES ('31', '女布', '月布');
INSERT INTO `buddy_book_update` VALUES ('32', '冰鳅', '冰龙');
INSERT INTO `buddy_book_update` VALUES ('33', '冰龙', '雪女');
INSERT INTO `buddy_book_update` VALUES ('34', '星蛋', '星妞');
INSERT INTO `buddy_book_update` VALUES ('35', '轻轻花', '风精灵');
INSERT INTO `buddy_book_update` VALUES ('36', '地狱犬', '魔羊');
INSERT INTO `buddy_book_update` VALUES ('37', '魔羊', '大恶魔');
INSERT INTO `buddy_book_update` VALUES ('38', '绿芽龙', '沙漠暴龙');
INSERT INTO `buddy_book_update` VALUES ('39', '臭美猫', '修罗女');
INSERT INTO `buddy_book_update` VALUES ('40', '咬力蚁', '蜻蜓龙');
INSERT INTO `buddy_book_update` VALUES ('41', '石脑龙', '飞天血蛤');
INSERT INTO `buddy_book_update` VALUES ('42', '铁手臂', '机器手');
INSERT INTO `buddy_book_update` VALUES ('43', '机器手', '钢螃蟹');

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
  CONSTRAINT `buddy_key1` FOREIGN KEY (`buddy`) REFERENCES `buddy_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `id_key1` FOREIGN KEY (`id`) REFERENCES `buddy_map_lbs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=gb2312 COMMENT='巴迪分布图 巴迪';

-- ----------------------------
-- Records of buddy_map_pet
-- ----------------------------
INSERT INTO `buddy_map_pet` VALUES ('18', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('19', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('20', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('22', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('24', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('26', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('28', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('30', '爆炎龙');
INSERT INTO `buddy_map_pet` VALUES ('18', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('20', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('21', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('23', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('25', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('27', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('29', '怪蛙草');
INSERT INTO `buddy_map_pet` VALUES ('31', '怪蛙草');

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
) ENGINE=InnoDB AUTO_INCREMENT=231 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of ex_effect
-- ----------------------------
INSERT INTO `ex_effect` VALUES ('1', '烧伤', '攻击减半');
INSERT INTO `ex_effect` VALUES ('2', '麻痹', '速度降低75%');
INSERT INTO `ex_effect` VALUES ('3', '冰封', '不能行动0~6回合，每回合25%几率解除');
INSERT INTO `ex_effect` VALUES ('4', '催眠', '不能行动0~6回合，每回合25%几率解除');
INSERT INTO `ex_effect` VALUES ('5', '中毒', '每回合损失最大体力8%');
INSERT INTO `ex_effect` VALUES ('11', '1击毙', '1击毙');
INSERT INTO `ex_effect` VALUES ('12', '睡觉', '睡3回合，HP全满，状态解除');
INSERT INTO `ex_effect` VALUES ('13', '混乱', '50%攻击自己');
INSERT INTO `ex_effect` VALUES ('14', '每回合吸收1/8体力', '每回合吸收1/8体力');
INSERT INTO `ex_effect` VALUES ('15', '留场', '不能换人和逃跑');
INSERT INTO `ex_effect` VALUES ('16', '七伤拳', 'HP越低威力越大20~200');
INSERT INTO `ex_effect` VALUES ('17', '交换物品', null);
INSERT INTO `ex_effect` VALUES ('18', '不能连续使用相同技能', null);
INSERT INTO `ex_effect` VALUES ('19', '3回合不能使用变化技能', null);
INSERT INTO `ex_effect` VALUES ('20', '对方HP减少到与自己相同', null);
INSERT INTO `ex_effect` VALUES ('21', '对方下回合被催眠', null);
INSERT INTO `ex_effect` VALUES ('23', '非战斗使用可引出稀有巴迪', null);
INSERT INTO `ex_effect` VALUES ('24', '3回合不能更换技能', null);
INSERT INTO `ex_effect` VALUES ('25', '双方HP平分', null);
INSERT INTO `ex_effect` VALUES ('26', '替身', '损失1/4HP制造');
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
INSERT INTO `ex_effect` VALUES ('49', '日月精华', '回复50%HP,晴天2倍，其它天气减半');
INSERT INTO `ex_effect` VALUES ('52', '对方不能使用我方会的技能', null);
INSERT INTO `ex_effect` VALUES ('56', '使上场巴迪受到1/8伤害', null);
INSERT INTO `ex_effect` VALUES ('57', '电系威力*1.5', null);
INSERT INTO `ex_effect` VALUES ('61', '非战斗使用可以吞掉物品，法宝转换为正能量', null);
INSERT INTO `ex_effect` VALUES ('62', '交换巴迪', null);
INSERT INTO `ex_effect` VALUES ('63', '起死回生', '恢复50%体力');
INSERT INTO `ex_effect` VALUES ('64', '拉对手一起死', null);
INSERT INTO `ex_effect` VALUES ('66', '强制退场', null);
INSERT INTO `ex_effect` VALUES ('68', '胆量', '普和斗属性技能可打中鬼系巴迪');
INSERT INTO `ex_effect` VALUES ('69', '解除异常', null);
INSERT INTO `ex_effect` VALUES ('70', '吸引全场火力', null);
INSERT INTO `ex_effect` VALUES ('72', '交换巴迪', null);
INSERT INTO `ex_effect` VALUES ('73', '梦话', '睡着时，随机使用技能');
INSERT INTO `ex_effect` VALUES ('74', '免疫1击毙', null);
INSERT INTO `ex_effect` VALUES ('75', '下回合技能必中', null);
INSERT INTO `ex_effect` VALUES ('76', '永久复制对手技能', null);
INSERT INTO `ex_effect` VALUES ('77', '听过的巴迪，3回合内不换下，则濒死', null);
INSERT INTO `ex_effect` VALUES ('79', '免疫技能无效', null);
INSERT INTO `ex_effect` VALUES ('80', '变身', '变成对手');
INSERT INTO `ex_effect` VALUES ('86', '霸气', '使对方害怕');
INSERT INTO `ex_effect` VALUES ('87', '先手', null);
INSERT INTO `ex_effect` VALUES ('88', '暴击', null);
INSERT INTO `ex_effect` VALUES ('89', '吸血', null);
INSERT INTO `ex_effect` VALUES ('90', '反噬', '1/3伤害反弹');
INSERT INTO `ex_effect` VALUES ('91', '再攻击1次', null);
INSERT INTO `ex_effect` VALUES ('92', '下回合不能行动', null);
INSERT INTO `ex_effect` VALUES ('93', '无盲点', null);
INSERT INTO `ex_effect` VALUES ('94', 'AOE', null);
INSERT INTO `ex_effect` VALUES ('95', '乱舞', '连续攻击2~3回合，然后混乱');
INSERT INTO `ex_effect` VALUES ('96', '非战斗使用可拆除建筑物', null);
INSERT INTO `ex_effect` VALUES ('98', '无视对方能力变化', null);
INSERT INTO `ex_effect` VALUES ('101', '蓄力', '先手蓄力后手攻击，攻击前受到攻击则失败');
INSERT INTO `ex_effect` VALUES ('102', '后手', null);
INSERT INTO `ex_effect` VALUES ('107', '威力翻倍', null);
INSERT INTO `ex_effect` VALUES ('108', '给敌人留下至少1点体力', null);
INSERT INTO `ex_effect` VALUES ('109', '不能更换技能', null);
INSERT INTO `ex_effect` VALUES ('112', '太阳能', '不是晴天，需要蓄力1回合');
INSERT INTO `ex_effect` VALUES ('113', '攻击延后2回合', '攻击延后2回合');
INSERT INTO `ex_effect` VALUES ('114', '物理伤害', null);
INSERT INTO `ex_effect` VALUES ('115', '非战斗使用可摘取星空中的法宝', null);
INSERT INTO `ex_effect` VALUES ('116', '消耗1个上帝之手', null);
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
INSERT INTO `ex_effect` VALUES ('137', '免疫反噬', null);
INSERT INTO `ex_effect` VALUES ('138', '免疫弱化', null);
INSERT INTO `ex_effect` VALUES ('139', '大幅强化物攻', '');
INSERT INTO `ex_effect` VALUES ('140', '大幅强化物防', '');
INSERT INTO `ex_effect` VALUES ('141', '大幅强化特攻', '');
INSERT INTO `ex_effect` VALUES ('142', '大幅强化特防', '');
INSERT INTO `ex_effect` VALUES ('143', '大幅强化速度', '');
INSERT INTO `ex_effect` VALUES ('144', '大幅弱化物攻', '');
INSERT INTO `ex_effect` VALUES ('145', '大幅弱化物防', '');
INSERT INTO `ex_effect` VALUES ('146', '大幅弱化特攻', '');
INSERT INTO `ex_effect` VALUES ('147', '大幅弱化特防', '');
INSERT INTO `ex_effect` VALUES ('148', '大幅弱化速度', '');
INSERT INTO `ex_effect` VALUES ('149', '微量恢复', '每回合结束恢复1/16体力');
INSERT INTO `ex_effect` VALUES ('152', '永久雨天', null);
INSERT INTO `ex_effect` VALUES ('153', '永久沙暴', null);
INSERT INTO `ex_effect` VALUES ('154', '永久晴天', null);
INSERT INTO `ex_effect` VALUES ('155', '永久冰雹', null);
INSERT INTO `ex_effect` VALUES ('156', '免疫暴击', null);
INSERT INTO `ex_effect` VALUES ('157', '免疫麻痹', null);
INSERT INTO `ex_effect` VALUES ('158', '免疫草系', null);
INSERT INTO `ex_effect` VALUES ('159', '免疫水系', null);
INSERT INTO `ex_effect` VALUES ('160', '免疫火系', null);
INSERT INTO `ex_effect` VALUES ('161', '免疫电系', null);
INSERT INTO `ex_effect` VALUES ('162', '免疫地系', null);
INSERT INTO `ex_effect` VALUES ('163', '免疫烧伤', null);
INSERT INTO `ex_effect` VALUES ('164', '免疫催眠', null);
INSERT INTO `ex_effect` VALUES ('165', '免疫冰封', null);
INSERT INTO `ex_effect` VALUES ('166', '免疫中毒', null);
INSERT INTO `ex_effect` VALUES ('167', '免疫霸气', null);
INSERT INTO `ex_effect` VALUES ('168', '免疫特效', null);
INSERT INTO `ex_effect` VALUES ('169', '只受攻击伤害', null);
INSERT INTO `ex_effect` VALUES ('170', '火系提升', null);
INSERT INTO `ex_effect` VALUES ('171', '水系提升', null);
INSERT INTO `ex_effect` VALUES ('172', '反转回复', null);
INSERT INTO `ex_effect` VALUES ('175', '草系提升', null);
INSERT INTO `ex_effect` VALUES ('176', '虫系提升', null);
INSERT INTO `ex_effect` VALUES ('177', '重伤时发动', null);
INSERT INTO `ex_effect` VALUES ('178', '受到物理攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('179', '雨天特效', null);
INSERT INTO `ex_effect` VALUES ('180', '晴天特效', null);
INSERT INTO `ex_effect` VALUES ('181', '冰雹特效', null);
INSERT INTO `ex_effect` VALUES ('182', '沙暴特效', null);
INSERT INTO `ex_effect` VALUES ('183', '速度翻倍', null);
INSERT INTO `ex_effect` VALUES ('184', '复制特性', null);
INSERT INTO `ex_effect` VALUES ('185', '微量回复', '恢复1/16体力');
INSERT INTO `ex_effect` VALUES ('186', '冰火属性伤害减半', null);
INSERT INTO `ex_effect` VALUES ('187', '免疫留场', null);
INSERT INTO `ex_effect` VALUES ('188', '技能必中', null);
INSERT INTO `ex_effect` VALUES ('189', '害怕时发动', null);
INSERT INTO `ex_effect` VALUES ('190', '受恶系攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('191', '受水系攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('192', '受火系攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('193', '受电系攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('194', '受草系攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('195', '中毒发动', null);
INSERT INTO `ex_effect` VALUES ('197', '强力', '物攻*1.5');
INSERT INTO `ex_effect` VALUES ('198', '强攻', '特攻*1.5');
INSERT INTO `ex_effect` VALUES ('199', '强体', '物防*1.5');
INSERT INTO `ex_effect` VALUES ('200', '强防', '特防*1.5');
INSERT INTO `ex_effect` VALUES ('201', '强速', '速度*1.5');
INSERT INTO `ex_effect` VALUES ('202', '每回合消耗1/8体力', null);
INSERT INTO `ex_effect` VALUES ('203', '伤害提升', '伤害*1.5');
INSERT INTO `ex_effect` VALUES ('204', '威力小于60的技能发动', null);
INSERT INTO `ex_effect` VALUES ('205', '效果低于1倍时发动', null);
INSERT INTO `ex_effect` VALUES ('206', '反噬技能提升', null);
INSERT INTO `ex_effect` VALUES ('207', '伤害*1.3', null);
INSERT INTO `ex_effect` VALUES ('208', '圣斗士', '见过的技能无效');
INSERT INTO `ex_effect` VALUES ('209', '物理技能发动', null);
INSERT INTO `ex_effect` VALUES ('210', '回复1/3体力', null);
INSERT INTO `ex_effect` VALUES ('211', '后攻击时发动', null);
INSERT INTO `ex_effect` VALUES ('212', '穿透', '无视光墙，反射盾');
INSERT INTO `ex_effect` VALUES ('213', '反弹变化技能', null);
INSERT INTO `ex_effect` VALUES ('214', '地系提升', null);
INSERT INTO `ex_effect` VALUES ('215', '岩系提升', null);
INSERT INTO `ex_effect` VALUES ('216', '钢系提升', null);
INSERT INTO `ex_effect` VALUES ('217', '普系边飞行，且威力*130%', null);
INSERT INTO `ex_effect` VALUES ('219', '普系边冰，且威力*130%', null);
INSERT INTO `ex_effect` VALUES ('220', '飞行技能特效', null);
INSERT INTO `ex_effect` VALUES ('221', '恶系提升', null);
INSERT INTO `ex_effect` VALUES ('222', '速度*1.2', null);
INSERT INTO `ex_effect` VALUES ('223', '伤害*1.2', null);
INSERT INTO `ex_effect` VALUES ('224', '物攻*2', null);
INSERT INTO `ex_effect` VALUES ('226', '满HP时发动', null);
INSERT INTO `ex_effect` VALUES ('227', '闪避', null);
INSERT INTO `ex_effect` VALUES ('228', '无限反噬', '不会因反噬体力归0');
INSERT INTO `ex_effect` VALUES ('229', '完全恢复', '回复100%体力');
INSERT INTO `ex_effect` VALUES ('230', '控场技能持续8回合', null);

-- ----------------------------
-- Table structure for `item_book`
-- ----------------------------
DROP TABLE IF EXISTS `item_book`;
CREATE TABLE `item_book` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `name` varchar(14) NOT NULL COMMENT '名字',
  `coin` int(11) NOT NULL COMMENT '价值',
  `descript` varchar(60) NOT NULL COMMENT '描述',
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=166 DEFAULT CHARSET=gb2312 COMMENT='物品图鉴';

-- ----------------------------
-- Records of item_book
-- ----------------------------
INSERT INTO `item_book` VALUES ('1', '上帝之手', '50', '使用摘星术必须品，可摘取星空中的法宝');
INSERT INTO `item_book` VALUES ('2', '万能药', '50', '解除所有异常');
INSERT INTO `item_book` VALUES ('3', '人参', '100', '补满血');
INSERT INTO `item_book` VALUES ('4', '巴迪球', '0', '驯化巴迪兽');
INSERT INTO `item_book` VALUES ('5', '兴奋剂-攻', '200', '生命低于1/3时提升物攻');
INSERT INTO `item_book` VALUES ('6', '兴奋剂-特攻', '200', '生命低于1/3时提升特攻');
INSERT INTO `item_book` VALUES ('7', '兴奋剂-速度', '200', '生命低于1/3时提升速度');
INSERT INTO `item_book` VALUES ('8', '天地灵根', '100000', '每日生长出日用品');
INSERT INTO `item_book` VALUES ('9', '恶魔果实-普', '10000', '吃下后，学会所有普属性技能');
INSERT INTO `item_book` VALUES ('10', '恶魔果实-斗', '10000', '吃下后，学会所有斗属性技能');
INSERT INTO `item_book` VALUES ('11', '恶魔果实-鬼', '10000', '吃下后，学会所有鬼属性技能');
INSERT INTO `item_book` VALUES ('12', '恶魔果实-飞', '10000', '吃下后，学会所有飞属性技能');
INSERT INTO `item_book` VALUES ('13', '恶魔果实-虫', '10000', '吃下后，学会所有虫属性技能');
INSERT INTO `item_book` VALUES ('14', '恶魔果实-毒', '10000', '吃下后，学会所有毒属性技能');
INSERT INTO `item_book` VALUES ('15', '恶魔果实-地', '10000', '吃下后，学会所有地属性技能');
INSERT INTO `item_book` VALUES ('16', '恶魔果实-岩', '10000', '吃下后，学会所有岩属性技能');
INSERT INTO `item_book` VALUES ('17', '恶魔果实-钢', '10000', '吃下后，学会所有钢属性技能');
INSERT INTO `item_book` VALUES ('18', '恶魔果实-恶', '10000', '吃下后，学会所有恶属性技能');
INSERT INTO `item_book` VALUES ('19', '恶魔果实-龙', '10000', '吃下后，学会所有龙属性技能');
INSERT INTO `item_book` VALUES ('20', '恶魔果实-超', '10000', '吃下后，学会所有超属性技能');
INSERT INTO `item_book` VALUES ('21', '恶魔果实-水', '10000', '吃下后，学会所有水属性技能');
INSERT INTO `item_book` VALUES ('22', '恶魔果实-火', '10000', '吃下后，学会所有火属性技能');
INSERT INTO `item_book` VALUES ('23', '恶魔果实-冰', '10000', '吃下后，学会所有冰属性技能');
INSERT INTO `item_book` VALUES ('24', '恶魔果实-电', '10000', '吃下后，学会所有电属性技能');
INSERT INTO `item_book` VALUES ('25', '恶魔果实-草', '10000', '吃下后，学会所有草属性技能');
INSERT INTO `item_book` VALUES ('26', '恶魔果实-妖', '10000', '吃下后，学会所有妖属性技能');
INSERT INTO `item_book` VALUES ('27', '王冠', '10000', '15%使对方害怕');
INSERT INTO `item_book` VALUES ('28', '光粉', '10000', '20%闪避');
INSERT INTO `item_book` VALUES ('29', '毒珠', '10000', '上场中毒');
INSERT INTO `item_book` VALUES ('30', '火珠', '10000', '上场烧伤');
INSERT INTO `item_book` VALUES ('31', '放大镜', '20000', '必中');
INSERT INTO `item_book` VALUES ('32', '四叶草', '10000', '15%暴击');
INSERT INTO `item_book` VALUES ('33', '针管', '30000', '50%吸血');
INSERT INTO `item_book` VALUES ('34', '先制爪', '10000', '20%先制');
INSERT INTO `item_book` VALUES ('35', '剩饭', '30000', '1/16回血');
INSERT INTO `item_book` VALUES ('36', '免死金牌', '30000', '忍耐');
INSERT INTO `item_book` VALUES ('37', '磁盘', '50000', '场地效果持续8回合');
INSERT INTO `item_book` VALUES ('38', '专爱头巾', '50000', '携带后，物攻*1.5，不能换技能');
INSERT INTO `item_book` VALUES ('39', '生命玉', '30000', '携带后，伤害*1.25，每次攻击损失1/8体力');

-- ----------------------------
-- Table structure for `item_book_effect`
-- ----------------------------
DROP TABLE IF EXISTS `item_book_effect`;
CREATE TABLE `item_book_effect` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `item` varchar(255) NOT NULL COMMENT '特性',
  `step` varchar(255) DEFAULT '战斗' COMMENT '触发时机',
  `effect` varchar(255) DEFAULT '霸气' COMMENT '特效',
  `probability` int(11) DEFAULT '100' COMMENT '概率',
  `agent` tinyint(4) DEFAULT '0' COMMENT '作用对象：0对方，1自己，2全场，3天气',
  PRIMARY KEY (`id`),
  KEY `step_name1` (`step`),
  KEY `effect_name1` (`effect`),
  KEY `skill_name1` (`item`),
  CONSTRAINT `item_book_effect_ibfk_3` FOREIGN KEY (`effect`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `item_book_effect_ibfk_1` FOREIGN KEY (`step`) REFERENCES `battle_step` (`step`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `item_book_effect_ibfk_2` FOREIGN KEY (`item`) REFERENCES `item_book` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of item_book_effect
-- ----------------------------
INSERT INTO `item_book_effect` VALUES ('1', '万能药', '战斗', '重伤时发动', '100', '1');
INSERT INTO `item_book_effect` VALUES ('2', '万能药', '战斗', '解除异常', '100', '1');
INSERT INTO `item_book_effect` VALUES ('3', '人参', '战斗', '重伤时发动', '100', '1');
INSERT INTO `item_book_effect` VALUES ('4', '人参', '战斗', '完全恢复', '100', '1');
INSERT INTO `item_book_effect` VALUES ('5', '兴奋剂-攻', '战斗', '重伤时发动', '100', '1');
INSERT INTO `item_book_effect` VALUES ('6', '兴奋剂-攻', '战斗', '强化物攻', '100', '1');
INSERT INTO `item_book_effect` VALUES ('7', '兴奋剂-特攻', '战斗', '重伤时发动', '100', '1');
INSERT INTO `item_book_effect` VALUES ('8', '兴奋剂-特攻', '战斗', '强化特攻', '100', '1');
INSERT INTO `item_book_effect` VALUES ('9', '兴奋剂-速度', '战斗', '重伤时发动', '100', '1');
INSERT INTO `item_book_effect` VALUES ('10', '兴奋剂-速度', '战斗', '强化速度', '100', '1');
INSERT INTO `item_book_effect` VALUES ('11', '王冠', '战斗', '霸气', '15', '0');
INSERT INTO `item_book_effect` VALUES ('12', '光粉', '战斗', '闪避', '20', '1');
INSERT INTO `item_book_effect` VALUES ('13', '毒珠', '换人', '中毒', '100', '1');
INSERT INTO `item_book_effect` VALUES ('14', '火珠', '换人', '烧伤', '100', '1');
INSERT INTO `item_book_effect` VALUES ('15', '放大镜', '战斗', '技能必中', '100', '1');
INSERT INTO `item_book_effect` VALUES ('16', '四叶草', '战斗', '暴击', '15', '0');
INSERT INTO `item_book_effect` VALUES ('17', '针管', '战斗', '吸血', '100', '1');
INSERT INTO `item_book_effect` VALUES ('18', '先制爪', '准备战斗', '先手', '20', '1');
INSERT INTO `item_book_effect` VALUES ('19', '剩饭', '回合结束', '微量回复', '100', '1');
INSERT INTO `item_book_effect` VALUES ('20', '免死金牌', '战斗', '满HP时发动', '100', '1');
INSERT INTO `item_book_effect` VALUES ('21', '免死金牌', '战斗', '免疫1击毙', '100', '1');
INSERT INTO `item_book_effect` VALUES ('22', '磁盘', '战斗', '控场技能持续8回合', '100', '1');
INSERT INTO `item_book_effect` VALUES ('23', '专爱头巾', '战斗', '强力', '100', '1');
INSERT INTO `item_book_effect` VALUES ('24', '专爱头巾', '战斗', '不能更换技能', '100', '1');
INSERT INTO `item_book_effect` VALUES ('25', '生命玉', '战斗', '伤害*1.3', '100', '1');
INSERT INTO `item_book_effect` VALUES ('26', '生命玉', '战斗', '反噬', '100', '1');

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
-- Table structure for `skill_book_effect`
-- ----------------------------
DROP TABLE IF EXISTS `skill_book_effect`;
CREATE TABLE `skill_book_effect` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `skill` varchar(255) NOT NULL COMMENT '技能',
  `step` varchar(255) DEFAULT '战斗' COMMENT '触发时机',
  `effect` varchar(255) DEFAULT '霸气' COMMENT '特效',
  `probability` int(11) DEFAULT '100' COMMENT '概率',
  `agent` tinyint(4) DEFAULT '0' COMMENT '作用对象：0对方，1自己，2全场，3天气',
  PRIMARY KEY (`id`),
  KEY `step_name1` (`step`),
  KEY `effect_name1` (`effect`),
  KEY `skill_name1` (`skill`),
  CONSTRAINT `step_name1` FOREIGN KEY (`step`) REFERENCES `battle_step` (`step`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `effect_name1` FOREIGN KEY (`effect`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `skill_name1` FOREIGN KEY (`skill`) REFERENCES `skill_book_info` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=254 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of skill_book_effect
-- ----------------------------
INSERT INTO `skill_book_effect` VALUES ('1', '绝对零度', '战斗', '1击毙', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('2', '冰刀', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('3', '雪崩', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('4', '急冻拳', '战斗', '冰封', '10', '0');
INSERT INTO `skill_book_effect` VALUES ('5', '冰雹', '战斗', '5回合冰雹天气，冰属性以外的巴迪每回合开始受到1/16伤害', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('6', '黑雾', '战斗', '全体能力等级复原', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('7', '暗香突袭', '战斗', '暴击', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('8', '极光', '战斗', '弱化速度', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('9', '急冻光线', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('10', '急冻光线', '战斗', '冰封', '10', '0');
INSERT INTO `skill_book_effect` VALUES ('11', '暴风雪', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('12', '暴风雪', '战斗', '冰封', '10', '0');
INSERT INTO `skill_book_effect` VALUES ('13', '飞叶快刀', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('14', '木角', '战斗', '吸血', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('15', '种子爆弹', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('16', '叶刃斩', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('17', '木槌', '战斗', '反噬', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('18', '寄生种子', '战斗', '每回合吸收1/8体力', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('19', '香甜气息', '战斗', '催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('20', '香甜气息', '非战斗', '非战斗使用可引出稀有巴迪', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('21', '草笛', '战斗', '催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('24', '芳香疗法', '战斗', '全体异常解除', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('25', '光合作用', '战斗', '日月精华', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('26', '睡眠粉', '战斗', '催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('27', '麻痹粉', '战斗', '麻痹', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('28', '飘花淡雪浮香吹', '准备战斗', '后手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('29', '飘花淡雪浮香吹', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('30', '飘花淡雪浮香吹', '战斗', '冰封', '10', '0');
INSERT INTO `skill_book_effect` VALUES ('31', '飘花淡雪浮香吹', '战斗', '强制退场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('32', '能源球', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('33', '种子闪光弹', '战斗', '大幅弱化特防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('34', '花瓣舞', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('35', '阳光烈焰', '战斗', '太阳能', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('36', '叶风暴', '战斗', '大幅弱化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('37', '精神切割', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('38', '心灵压迫', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('39', '催眠术', '战斗', '催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('40', '高速移动', '战斗', '大幅强化速度', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('41', '栅栏', '战斗', '大幅强化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('42', '光之壁', '战斗', '5回合特殊伤害减半', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('44', '反射盾', '战斗', '5回合物理伤害减半', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('45', '超级健忘', '战斗', '大幅强化特防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('46', '封印', '战斗', '对方不能使用我方会的技能', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('47', '冥想', '战斗', '强化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('48', '冥想', '战斗', '强化特防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('49', '欺骗空间', '战斗', '5回合速度慢的先行动', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('50', '睡觉', '战斗', '睡觉', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('51', '精神干扰', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('52', '精神冲击', '战斗', '物理伤害', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('53', '精神病', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('54', '预知未来', '战斗', '攻击延后2回合', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('55', '精神增压', '战斗', '大幅弱化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('56', '蜻蜓回转', '战斗', '交换巴迪', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('57', '十字剪', '战斗', '暴击', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('58', '蜂涌', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('59', '终极针刺', '战斗', '下回合不能行动', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('60', '蜘蛛网', '战斗', '留场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('61', '萤火', '战斗', '大幅强化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('62', '蝶之舞', '战斗', '强化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('63', '蝶之舞', '战斗', '强化特防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('64', '蝶之舞', '战斗', '强化速度', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('65', '信号光线', '战斗', '强化特攻', '30', '1');
INSERT INTO `skill_book_effect` VALUES ('66', '虫鸣', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('67', '地裂', '战斗', '1击毙', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('68', '地震', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('69', '撒菱', '战斗', '使上场巴迪受到1/8伤害', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('70', '大地之力', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('71', '雷光拳', '战斗', '麻痹', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('72', '闪电突袭', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('73', '闪电突袭', '战斗', '反噬', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('74', '能量冲击', '战斗', '麻痹', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('75', '能量冲击', '战斗', '反噬', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('76', '电磁波', '战斗', '麻痹', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('77', '充电', '战斗', '强化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('78', '充电', '战斗', '电系威力*1.5', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('79', '交流电', '战斗', '交换巴迪', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('80', '十万伏特', '战斗', '麻痹', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('81', '打雷', '战斗', '麻痹', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('82', '七伤拳', '战斗', '七伤拳', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('83', '山岚', '战斗', '暴击', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('84', '音速拳', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('85', '碎石踢', '非战斗', '非战斗使用可拆除建筑物', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('86', '十字切', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('87', '进身击', '战斗', '弱化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('88', '进身击', '战斗', '弱化特防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('89', '气合拳', '战斗', '蓄力', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('90', '裸奔气合拳', '战斗', '蓄力', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('91', '巨大化', '战斗', '强化物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('92', '巨大化', '战斗', '强化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('93', '波动拳', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('94', '剧毒之牙', '战斗', '中毒', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('95', '毒突', '战斗', '弱化物防', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('96', '粉尘射击', '战斗', '中毒', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('97', '毒粉', '战斗', '中毒', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('98', '淤泥爆弹', '战斗', '中毒', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('99', '偷袭', '战斗', '暴击', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('100', '落井下石', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('101', '暗算', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('102', '咬碎', '战斗', '弱化物防', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('103', '试刀', '战斗', '暴击', '40', '0');
INSERT INTO `skill_book_effect` VALUES ('104', '吞噬', '战斗', '留场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('105', '吞噬', '非战斗', '非战斗使用可以吞掉物品，法宝转换为正能量', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('106', '磨爪子', '战斗', '强化物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('107', '磨爪子', '战斗', '提高暴击率', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('108', '黑洞', '战斗', '催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('109', '诡计', '战斗', '大幅强化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('110', '掉包', '战斗', '交换物品', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('111', '寻衅', '战斗', '不能连续使用相同技能', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('112', '挑拨', '战斗', '3回合不能使用变化技能', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('113', '黑夜爆裂', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('114', '恶念波动', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('115', '异次元冲锋', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('116', '燕反', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('117', '钻孔啄', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('118', '勇鸟', '战斗', '反噬', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('119', '风之伤', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('120', '吹飞', '准备战斗', '后手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('121', '吹飞', '战斗', '强制退场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('122', '羽栖', '战斗', '起死回生', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('123', '空气切割', '战斗', '霸气', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('124', '空气爆炸', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('125', '子弹拳', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('126', '铁头槌', '战斗', '霸气', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('127', '彗星拳', '战斗', '强化物攻', '30', '1');
INSERT INTO `skill_book_effect` VALUES ('128', '破灭之愿', '战斗', '攻击延后2回合', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('129', '变换齿轮', '战斗', '大幅强化速度', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('130', '铁壁', '战斗', '大幅强化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('131', '镜面射击', '战斗', '强化特攻', '30', '1');
INSERT INTO `skill_book_effect` VALUES ('132', '光栅加农', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('133', '鬼爪', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('134', '鬼影', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('135', '同归', '战斗', '拉对手一起死', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('136', '奇怪光', '战斗', '混乱', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('137', '影袭', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('138', '影子球', '战斗', '弱化特防', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('139', '火焰拳', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('140', '炽热推进', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('141', '炽热推进', '战斗', '反噬', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('142', '鬼火', '战斗', '烧伤', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('143', '晴天', '战斗', '5回合晴天', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('144', '喷射火焰', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('145', '喷射火焰', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('146', '大字爆', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('147', '大字爆', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('148', '燃烧殆尽', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('149', '燃烧殆尽', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('150', '燃烧殆尽', '战斗', '大幅弱化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('151', '龙之尾', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('152', '龙之尾', '战斗', '强制退场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('153', '龙之冲锋', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('154', '逆鳞', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('155', '龙之舞', '战斗', '强化物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('156', '龙之舞', '战斗', '强化速度', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('157', '龙之吐息', '战斗', '麻痹', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('158', '龙星群', '战斗', '大幅弱化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('159', '垂死挣扎', '战斗', '七伤拳', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('160', '清场', '战斗', '清除钉子，减伤，天气等场地效果', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('161', '逆刃刀', '战斗', '给敌人留下至少1点体力', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('162', '旋风腿', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('163', '旋风腿', '战斗', '再攻击1次', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('164', '电光石火', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('165', '双重重击', '战斗', '再攻击1次', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('166', '空元气', '战斗', '自身异常时发动特效', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('167', '空元气', '战斗', '威力翻倍', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('168', '劈开', '战斗', '暴击', '40', '0');
INSERT INTO `skill_book_effect` VALUES ('169', '昏迷拳', '战斗', '混乱', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('170', '崩击爪', '战斗', '弱化物防', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('171', '神速', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('172', '必杀门牙', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('173', '泰山压顶', '战斗', '麻痹', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('174', '铁头功', '战斗', '霸气', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('175', '舍身撞', '战斗', '反噬', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('176', '横冲直撞', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('177', '亿万冲击', '战斗', '下回合不能行动', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('178', '最终手段', '战斗', '不能更换技能', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('179', '最终手段', '战斗', '留场', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('180', '最终手段', '战斗', '大幅弱化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('181', '最终手段', '战斗', '弱化特防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('182', '自爆', '战斗', '1击毙', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('183', '大爆炸', '战斗', '1击毙', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('184', '吼叫', '准备战斗', '后手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('185', '吼叫', '战斗', '强制退场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('186', '幸运光环', '战斗', '5回合不被命中要害', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('187', '吠叫', '战斗', '强化物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('188', '黑眼', '战斗', '留场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('189', '嗅觉', '战斗', '胆量', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('190', '偷懒', '战斗', '起死回生', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('191', '清新', '战斗', '解除异常', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('192', '莽撞', '战斗', '对方HP减少到与自己相同', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('193', '哈欠', '战斗', '对方下回合被催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('194', '吸引火力', '战斗', '吸引全场火力', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('195', '吸引火力', '非战斗', '非战斗使用可引出稀有巴迪', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('196', '晨光', '战斗', '日月精华', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('197', '鼓掌', '战斗', '3回合不能更换技能', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('198', '分享痛楚', '战斗', '双方HP平分', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('199', '治愈铃声', '战斗', '全体异常解除', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('200', '梦话', '战斗', '梦话', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('201', '吃奶', '战斗', '起死回生', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('202', '忍耐', '战斗', '满HP时发动', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('203', '忍耐', '战斗', '免疫1击毙', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('204', '心眼', '战斗', '下回合技能必中', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('205', '素描', '战斗', '永久复制对手技能', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('207', '末日歌', '战斗', '听过的巴迪，3回合内不换下，则濒死', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('208', '吃撑了', '战斗', '极限物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('209', '保护', '战斗', '免疫技能无效', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('210', '替身', '战斗', '替身', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('211', '变身', '战斗', '变身', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('212', '蛇瞪眼', '战斗', '麻痹', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('213', '恶魔吻', '战斗', '催眠', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('214', '聚气', '战斗', '提高暴击率', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('215', '硬梆梆', '战斗', '大幅强化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('216', '生长', '战斗', '强化物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('217', '生长', '战斗', '强化特攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('218', '剪刀断头台', '战斗', '1击毙', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('219', '剑舞', '战斗', '大幅强化物攻', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('220', '秘密力量', '非战斗', '非战斗使用建造秘密基地', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('221', '三角攻击', '战斗', '麻痹', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('222', '三角攻击', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('223', '三角攻击', '战斗', '冰封', '10', '0');
INSERT INTO `skill_book_effect` VALUES ('224', '爆音波', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('225', '破坏死光', '战斗', '下回合不能行动', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('226', '摘星术', '战斗', '消耗1个上帝之手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('227', '摘星术', '非战斗', '非战斗使用可摘取星空中的法宝', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('228', '鲤鱼摆尾', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('229', '鲤鱼摆尾', '战斗', '强制退场', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('230', '潜水', '战斗', '暴击', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('231', '出水一击', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('232', '跃龙门', '战斗', '霸气', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('233', '激流涌进', '战斗', '乱舞', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('234', '高压水炮', '战斗', '下回合不能行动', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('235', '急救液', '战斗', '5回合，每回合回复8%HP', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('236', '求雨', '战斗', '5回合雨天', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('237', '缩壳', '战斗', '大幅强化物防', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('238', '沸水', '战斗', '烧伤', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('239', '冲浪', '战斗', 'AOE', '100', '2');
INSERT INTO `skill_book_effect` VALUES ('240', '岩崩', '战斗', '霸气', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('241', '石刃', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('242', '岩石炮', '战斗', '下回合不能行动', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('243', '沙暴', '战斗', '5回合沙暴天气，地岩钢以外属性巴迪受到1/16伤害', '100', '3');
INSERT INTO `skill_book_effect` VALUES ('244', '岩切', '战斗', '大幅强化速度', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('245', '原始力量', '战斗', '暴击', '20', '0');
INSERT INTO `skill_book_effect` VALUES ('246', '力量宝石', '战斗', '强化特攻', '30', '1');
INSERT INTO `skill_book_effect` VALUES ('247', '吸精', '战斗', '吸血', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('248', '嬉戏', '战斗', '混乱', '30', '0');
INSERT INTO `skill_book_effect` VALUES ('249', '天使吻', '战斗', '混乱', '100', '0');
INSERT INTO `skill_book_effect` VALUES ('250', '月光', '战斗', '日月精华', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('251', '妖风', '准备战斗', '先手', '100', '1');
INSERT INTO `skill_book_effect` VALUES ('252', '月影斩', '战斗', '强化特攻', '30', '1');
INSERT INTO `skill_book_effect` VALUES ('253', '破灭之光', '战斗', '反噬', '100', '1');

-- ----------------------------
-- Table structure for `skill_book_info`
-- ----------------------------
DROP TABLE IF EXISTS `skill_book_info`;
CREATE TABLE `skill_book_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(14) NOT NULL COMMENT '名字，最大7汉字',
  `race` varchar(11) NOT NULL COMMENT '属性',
  `power` int(11) NOT NULL COMMENT '威力0~300',
  `type` int(11) DEFAULT '1' COMMENT '技能类型：\r\n1物理 2变化 3特殊',
  `hitRate` int(11) DEFAULT '100' COMMENT '命中率30~101， 101为必中技能',
  `isMapSkill` int(11) unsigned zerofill DEFAULT '00000000000' COMMENT '是否地图技能：\r\n1是 0不是',
  `descript` varchar(60) NOT NULL COMMENT '描述，最大60byte',
  PRIMARY KEY (`id`),
  KEY `race_name` (`race`),
  KEY `name` (`name`),
  CONSTRAINT `race_name` FOREIGN KEY (`race`) REFERENCES `race` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=535 DEFAULT CHARSET=gb2312 COMMENT='技能图鉴';

-- ----------------------------
-- Records of skill_book_info
-- ----------------------------
INSERT INTO `skill_book_info` VALUES ('10', '秘密力量', '普', '70', '3', '100', '00000000001', '非战斗使用可以建造秘密基地');
INSERT INTO `skill_book_info` VALUES ('11', '摘星术', '普', '180', '3', '100', '00000000001', '使用亿万冲击波攻击对手，非战斗使用可摘取星空中的法宝');
INSERT INTO `skill_book_info` VALUES ('12', '吞噬', '恶', '0', '2', '101', '00000000001', '禁止换人和逃跑。非战斗使用可以吞掉物品，法宝转换为正能量');
INSERT INTO `skill_book_info` VALUES ('14', '逆刃刀', '普', '40', '1', '100', '00000000000', '敌人至少留下1点体力');
INSERT INTO `skill_book_info` VALUES ('15', '爆音波', '普', '90', '3', '100', '00000000000', '全体攻击');
INSERT INTO `skill_book_info` VALUES ('16', '吼叫', '普', '0', '2', '101', '00000000000', '最后出手，强制对方退场');
INSERT INTO `skill_book_info` VALUES ('19', '舍身撞', '普', '120', '1', '100', '00000000000', '1/3反伤');
INSERT INTO `skill_book_info` VALUES ('26', '亿万冲击', '普', '150', '1', '90', '00000000000', '1回合不能行动，可换人');
INSERT INTO `skill_book_info` VALUES ('28', '最终手段', '普', '160', '1', '100', '00000000000', '使用后不能进行以下操作，更换技能，交换巴迪，逃跑。并大幅弱化自身物防');
INSERT INTO `skill_book_info` VALUES ('30', '幸运光环', '普', '0', '2', '101', '00000000000', '5回合不被命中要害');
INSERT INTO `skill_book_info` VALUES ('34', '吠叫', '普', '0', '2', '101', '00000000000', '强化物攻');
INSERT INTO `skill_book_info` VALUES ('35', '黑眼', '普', '0', '2', '101', '00000000000', '防止对方换人和逃跑');
INSERT INTO `skill_book_info` VALUES ('36', '嗅觉', '普', '0', '2', '101', '00000000000', '普和斗属性技能可打中鬼系巴迪');
INSERT INTO `skill_book_info` VALUES ('37', '崩击爪', '普', '75', '1', '95', '00000000000', '20%弱化物防');
INSERT INTO `skill_book_info` VALUES ('39', '偷懒', '普', '0', '2', '101', '00000000000', '回复50%体力');
INSERT INTO `skill_book_info` VALUES ('41', '清新', '普', '0', '2', '101', '00000000000', '解除自身异常');
INSERT INTO `skill_book_info` VALUES ('42', '莽撞', '普', '0', '2', '100', '00000000000', '对方HP减少到与自己相同');
INSERT INTO `skill_book_info` VALUES ('43', '哈欠', '普', '0', '2', '100', '00000000000', '对方下回合被催眠');
INSERT INTO `skill_book_info` VALUES ('45', '吸引火力', '普', '0', '2', '101', '00000000001', '吸引全场火力，非战斗使用可引出稀有巴迪');
INSERT INTO `skill_book_info` VALUES ('47', '空元气', '普', '70', '1', '100', '00000000000', '自身异常状态时威力翻倍');
INSERT INTO `skill_book_info` VALUES ('50', '神速', '普', '80', '1', '100', '00000000000', '先制+5');
INSERT INTO `skill_book_info` VALUES ('52', '晨光', '普', '0', '2', '101', '00000000000', '回复50%HP,晴天2倍，其它天气减半');
INSERT INTO `skill_book_info` VALUES ('54', '清场', '普', '20', '1', '100', '00000000000', '清除钉子，种子，缠绕');
INSERT INTO `skill_book_info` VALUES ('55', '鼓掌', '普', '0', '2', '100', '00000000000', '3回合不能更换技能');
INSERT INTO `skill_book_info` VALUES ('57', '分享痛楚', '普', '0', '2', '100', '00000000000', '双方HP平分');
INSERT INTO `skill_book_info` VALUES ('59', '治愈铃声', '普', '0', '2', '101', '00000000000', '我方全体异常解除');
INSERT INTO `skill_book_info` VALUES ('60', '梦话', '普', '0', '2', '101', '00000000000', '睡着时，随机使用技能');
INSERT INTO `skill_book_info` VALUES ('61', '吃奶', '普', '0', '2', '101', '00000000000', '回复50%体力');
INSERT INTO `skill_book_info` VALUES ('63', '忍耐', '普', '0', '2', '101', '00000000000', '满HP时免疫1击毙，连用成功率降低');
INSERT INTO `skill_book_info` VALUES ('64', '心眼', '普', '0', '2', '101', '00000000000', '下回合技能必中');
INSERT INTO `skill_book_info` VALUES ('65', '素描', '普', '0', '2', '101', '00000000000', '永久复制对手技能');
INSERT INTO `skill_book_info` VALUES ('66', '末日歌', '普', '0', '2', '101', '00000000000', '听过的巴迪，3回合内不换下，则濒死');
INSERT INTO `skill_book_info` VALUES ('67', '吃撑了', '普', '0', '2', '101', '00000000000', '极限物攻');
INSERT INTO `skill_book_info` VALUES ('68', '保护', '普', '0', '2', '101', '00000000000', '免疫技能，连用成功率降低');
INSERT INTO `skill_book_info` VALUES ('69', '垂死挣扎', '普', '0', '1', '100', '00000000000', 'HP越低威力越大20~200');
INSERT INTO `skill_book_info` VALUES ('70', '替身', '普', '0', '2', '100', '00000000000', '损失1/4HP制造替身');
INSERT INTO `skill_book_info` VALUES ('71', '劈开', '普', '70', '1', '100', '00000000000', '40%暴击');
INSERT INTO `skill_book_info` VALUES ('72', '三角攻击', '普', '70', '3', '100', '00000000000', '20%烧伤，20%麻痹，10%冰封');
INSERT INTO `skill_book_info` VALUES ('73', '必杀门牙', '普', '80', '1', '90', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('74', '大爆炸', '普', '250', '1', '100', '00000000000', '濒死');
INSERT INTO `skill_book_info` VALUES ('75', '昏迷拳', '普', '70', '1', '100', '00000000000', '30%混乱');
INSERT INTO `skill_book_info` VALUES ('76', '变身', '普', '0', '2', '101', '00000000000', '变成对手');
INSERT INTO `skill_book_info` VALUES ('77', '蛇瞪眼', '普', '0', '2', '100', '00000000000', '麻痹');
INSERT INTO `skill_book_info` VALUES ('78', '恶魔吻', '普', '0', '2', '75', '00000000000', '催眠对方');
INSERT INTO `skill_book_info` VALUES ('79', '高速星星', '普', '60', '3', '101', '00000000000', '必定命中');
INSERT INTO `skill_book_info` VALUES ('80', '自爆', '普', '200', '1', '100', '00000000000', '濒死');
INSERT INTO `skill_book_info` VALUES ('81', '聚气', '普', '0', '2', '101', '00000000000', '提高暴击率');
INSERT INTO `skill_book_info` VALUES ('82', '硬梆梆', '普 ', '0', '2', '100', '00000000000', '大幅强化物防');
INSERT INTO `skill_book_info` VALUES ('84', '电光石火', '普', '60', '1', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('85', '生长', '普', '0', '2', '101', '00000000000', '强化物攻，强化特攻');
INSERT INTO `skill_book_info` VALUES ('86', '怪力', '普', '90', '1', '100', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('87', '破坏死光', '普', '150', '3', '90', '00000000000', '1回合不能行动，可以换人');
INSERT INTO `skill_book_info` VALUES ('88', '横冲直撞', '普', '120', '1', '100', '00000000000', '连续使用2~3回合');
INSERT INTO `skill_book_info` VALUES ('89', '泰山压顶', '普', '80', '1', '100', '00000000000', '30%麻痹');
INSERT INTO `skill_book_info` VALUES ('90', '剪刀断头台', '普', '0', '2', '30', '00000000000', '1击毙');
INSERT INTO `skill_book_info` VALUES ('92', '铁头功', '普', '80', '1', '100', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('93', '百万飞踢', '普', '120', '1', '85', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('95', '吹飞', '飞', '0', '2', '101', '00000000000', '最后出手，强制对方退场');
INSERT INTO `skill_book_info` VALUES ('96', '剑舞', '普', '0', '2', '101', '00000000000', '大幅强化物攻');
INSERT INTO `skill_book_info` VALUES ('97', '旋风腿', '普', '40', '1', '100', '00000000000', '攻击2次，20%暴击');
INSERT INTO `skill_book_info` VALUES ('98', '百万拳击', '普', '100', '1', '100', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('99', '睡觉', '超', '0', '2', '101', '00000000000', '睡3回合，HP全满，状态解除');
INSERT INTO `skill_book_info` VALUES ('100', '双重重击', '普', '60', '1', '95', '00000000000', '攻击2次');
INSERT INTO `skill_book_info` VALUES ('101', '天使吻', '妖', '0', '2', '75', '00000000000', '混乱');
INSERT INTO `skill_book_info` VALUES ('102', '月光', '妖', '0', '2', '101', '00000000000', '回复50%HP,晴天2倍，其它天气减半');
INSERT INTO `skill_book_info` VALUES ('104', '吸精', '妖', '75', '1', '100', '00000000000', '吸取伤害的50%恢复自身');
INSERT INTO `skill_book_info` VALUES ('107', '嬉戏', '妖', '90', '1', '90', '00000000000', '30%混乱');
INSERT INTO `skill_book_info` VALUES ('108', '妖风', '妖', '60', '3', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('110', '月影斩', '妖', '95', '3', '100', '00000000000', '30%强化特攻');
INSERT INTO `skill_book_info` VALUES ('111', '破灭之光', '妖', '140', '3', '85', '00000000000', '1/3反伤');
INSERT INTO `skill_book_info` VALUES ('120', '山岚', '斗', '60', '1', '100', '00000000000', '必定暴击');
INSERT INTO `skill_book_info` VALUES ('121', '冲击波', '斗', '120', '3', '70', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('124', '波动拳', '斗', '90', '3', '90', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('126', '进身击', '斗', '120', '1', '100', '00000000000', '弱化自身物防，弱化自身特防');
INSERT INTO `skill_book_info` VALUES ('129', '巨大化', '斗', '0', '2', '101', '00000000000', '强化物攻，强化物防');
INSERT INTO `skill_book_info` VALUES ('130', '升龙拳', '斗', '90', '1', '90', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('133', '气合拳', '斗', '150', '1', '100', '00000000000', '先手蓄力后手攻击，攻击前受到攻击则失败');
INSERT INTO `skill_book_info` VALUES ('134', '裸奔气合拳', '斗', '200', '1', '100', '00000000000', '先手蓄力后手攻击，攻击前受到攻击则失败，无法与替身同时存在');
INSERT INTO `skill_book_info` VALUES ('135', '碎石踢', '斗', '100', '1', '95', '00000000000', '非战斗使用可拆除建筑物');
INSERT INTO `skill_book_info` VALUES ('136', '十字切', '斗', '100', '1', '80', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('138', '音速拳', '斗', '60', '1', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('139', '七伤拳', '斗', '0', '1', '100', '00000000000', 'HP越低威力越大20~200');
INSERT INTO `skill_book_info` VALUES ('147', '燃烧殆尽', '火', '140', '3', '90', '00000000000', '全体攻击，20%烧伤，大幅弱化特攻');
INSERT INTO `skill_book_info` VALUES ('151', '炽热推进', '火', '120', '1', '100', '00000000000', '1/3反伤，20%烧伤');
INSERT INTO `skill_book_info` VALUES ('155', '鬼火', '火', '0', '2', '95', '00000000000', '烧伤');
INSERT INTO `skill_book_info` VALUES ('157', '晴天', '火', '0', '2', '101', '00000000000', '5回合晴天');
INSERT INTO `skill_book_info` VALUES ('160', '大字爆', '火', '110', '3', '80', '00000000000', '全体攻击，20%烧伤');
INSERT INTO `skill_book_info` VALUES ('162', '喷射火焰', '火', '95', '3', '100', '00000000000', '全体攻击，20%烧伤');
INSERT INTO `skill_book_info` VALUES ('163', '火焰拳', '火', '80', '1', '100', '00000000000', '20%烧伤');
INSERT INTO `skill_book_info` VALUES ('164', '雪崩', '冰', '80', '1', '95', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('165', '暗香突袭', '冰', '60', '3', '100', '00000000000', '必定暴击');
INSERT INTO `skill_book_info` VALUES ('167', '冰刀', '冰', '60', '1', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('169', '绝对零度', '冰', '0', '1', '30', '00000000000', '1击毙');
INSERT INTO `skill_book_info` VALUES ('170', '冰雹', '冰', '0', '2', '101', '00000000000', '5回合冰雹天气，冰属性以外的巴迪每回合开始受到1/16伤害');
INSERT INTO `skill_book_info` VALUES ('173', '黑雾', '冰', '0', '2', '101', '00000000000', '全体能力等级复原');
INSERT INTO `skill_book_info` VALUES ('174', '极光', '冰', '80', '3', '100', '00000000000', '30%弱化速度');
INSERT INTO `skill_book_info` VALUES ('175', '暴风雪', '冰', '110', '3', '80', '00000000000', '全体攻击，10%冰封');
INSERT INTO `skill_book_info` VALUES ('176', '急冻光线', '冰', '95', '3', '100', '00000000000', '全体攻击，10%冰封');
INSERT INTO `skill_book_info` VALUES ('177', '急冻拳', '冰', '80', '1', '100', '00000000000', '10%冰封');
INSERT INTO `skill_book_info` VALUES ('181', '沸水', '水', '80', '3', '100', '00000000000', '20%烧伤');
INSERT INTO `skill_book_info` VALUES ('183', '鲤鱼摆尾', '水', '60', '1', '100', '00000000000', '强制换人，野战结束战斗');
INSERT INTO `skill_book_info` VALUES ('187', '急救液', '水', '0', '2', '101', '00000000000', '5回合，每回合回复8%HP');
INSERT INTO `skill_book_info` VALUES ('192', '高压水炮', '水', '140', '1', '90', '00000000000', '下回合不能行动');
INSERT INTO `skill_book_info` VALUES ('193', '潜水', '水', '60', '1', '100', '00000000000', '必定暴击');
INSERT INTO `skill_book_info` VALUES ('195', '求雨', '水', '0', '2', '101', '00000000000', '5回合雨天');
INSERT INTO `skill_book_info` VALUES ('197', '出水一击', '水', '80', '1', '100', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('199', '跃龙门', '水', '80', '1', '100', '00000000000', '30%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('200', '缩壳', '水', '0', '2', '101', '00000000000', '大幅强化物防');
INSERT INTO `skill_book_info` VALUES ('202', '冲浪', '水', '95', '3', '100', '00000000000', '全体攻击');
INSERT INTO `skill_book_info` VALUES ('203', '激流涌进', '水', '138', '1', '100', '00000000000', '连续使用3~5回合');
INSERT INTO `skill_book_info` VALUES ('204', '电磁波', '电', '0', '2', '100', '00000000000', '麻痹');
INSERT INTO `skill_book_info` VALUES ('205', '打雷', '电', '120', '3', '70', '00000000000', '20%麻痹');
INSERT INTO `skill_book_info` VALUES ('207', '闪电突袭', '电', '90', '1', '100', '00000000000', '先制+1，1/3反伤');
INSERT INTO `skill_book_info` VALUES ('209', '交流电', '电', '70', '3', '100', '00000000000', '攻击后交换巴迪出场');
INSERT INTO `skill_book_info` VALUES ('214', '能量冲击', '电', '120', '1', '100', '00000000000', '1/3反伤，20%麻痹');
INSERT INTO `skill_book_info` VALUES ('215', '充电', '电', '0', '2', '101', '00000000000', '强化特攻，电系威力*1.5');
INSERT INTO `skill_book_info` VALUES ('218', '十万伏特', '电', '95', '3', '100', '00000000000', '20%麻痹');
INSERT INTO `skill_book_info` VALUES ('220', '雷光拳', '电', '80', '1', '100', '00000000000', '20%麻痹');
INSERT INTO `skill_book_info` VALUES ('224', '空气切割', '飞', '80', '3', '95', '00000000000', '30%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('226', '勇鸟', '飞', '120', '1', '100', '00000000000', '1/3反伤');
INSERT INTO `skill_book_info` VALUES ('227', '羽栖', '飞', '0', '2', '101', '00000000000', '回复50%体力');
INSERT INTO `skill_book_info` VALUES ('229', '燕反', '飞', '70', '1', '101', '00000000000', '先制+1，必定命中');
INSERT INTO `skill_book_info` VALUES ('230', '风之伤', '飞', '120', '1', '100', '00000000000', '连续攻击2~3回合');
INSERT INTO `skill_book_info` VALUES ('231', '空气爆炸', '飞', '100', '3', '90', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('233', '钻孔啄', '飞', '80', '1', '100', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('234', '飞行', '飞', '90', '1', '100', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('237', '叶刃斩', '草', '90', '1', '100', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('239', '木角', '草', '75', '1', '100', '00000000000', '吸取伤害的50%恢复自身');
INSERT INTO `skill_book_info` VALUES ('240', '种子闪光弹', '草', '120', '3', '80', '00000000000', '大幅弱化自身特防');
INSERT INTO `skill_book_info` VALUES ('241', '木槌', '草', '120', '1', '100', '00000000000', '1/3反伤');
INSERT INTO `skill_book_info` VALUES ('242', '叶风暴', '草', '140', '3', '90', '00000000000', '大幅弱化自身特攻');
INSERT INTO `skill_book_info` VALUES ('243', '能源球', '草', '90', '3', '100', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('244', '种子爆弹', '草', '80', '1', '100', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('247', '草笛', '草', '0', '2', '55', '00000000000', '催眠对方');
INSERT INTO `skill_book_info` VALUES ('248', '芳香疗法', '草', '0', '2', '101', '00000000000', '全体异常解除');
INSERT INTO `skill_book_info` VALUES ('250', '光合作用', '草', '0', '2', '101', '00000000000', '回复50%HP,晴天2倍，其它天气减半');
INSERT INTO `skill_book_info` VALUES ('252', '花瓣舞', '草', '120', '3', '100', '00000000000', '连续使用2~3回合');
INSERT INTO `skill_book_info` VALUES ('253', '睡眠粉', '草', '0', '2', '75', '00000000000', '催眠对方');
INSERT INTO `skill_book_info` VALUES ('254', '麻痹粉', '草', '0', '2', '75', '00000000000', '麻痹');
INSERT INTO `skill_book_info` VALUES ('255', '阳光烈焰', '草', '120', '3', '100', '00000000000', '第1回合吸收阳光，第2回攻击。晴天直接攻击');
INSERT INTO `skill_book_info` VALUES ('257', '寄生种子', '草', '0', '2', '85', '00000000000', '每回合吸收1/8体力，对方换下解除');
INSERT INTO `skill_book_info` VALUES ('259', '飞叶快刀', '草', '60', '1', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('260', '飘花淡雪浮香吹', '草', '90', '3', '100', '00000000000', '最后出手，20%暴击，10%冰封，强制对手退场');
INSERT INTO `skill_book_info` VALUES ('263', '影袭', '鬼', '60', '3', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('264', '鬼爪', '鬼', '80', '1', '100', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('265', '鬼影', '鬼', '80', '1', '100', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('267', '影子球', '鬼', '90', '3', '100', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('268', '同归', '鬼', '0', '2', '101', '00000000000', '受到攻击体力归0时，拉对手一起倒下');
INSERT INTO `skill_book_info` VALUES ('269', '奇怪光', '鬼', '0', '2', '100', '00000000000', '混乱');
INSERT INTO `skill_book_info` VALUES ('271', '异次元冲锋', '恶', '120', '3', '100', '00000000000', '连续攻击2~3回合');
INSERT INTO `skill_book_info` VALUES ('273', '黑夜爆裂', '恶', '75', '3', '95', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('275', '磨爪子', '恶', '0', '2', '101', '00000000000', '强化物攻，提高暴击率');
INSERT INTO `skill_book_info` VALUES ('276', '黑洞', '恶', '0', '2', '80', '00000000000', '催眠对方');
INSERT INTO `skill_book_info` VALUES ('277', '诡计', '恶', '0', '2', '101', '00000000000', '大幅强化特攻');
INSERT INTO `skill_book_info` VALUES ('278', '掉包', '恶', '0', '2', '100', '00000000000', '交换物品');
INSERT INTO `skill_book_info` VALUES ('279', '试刀', '恶', '85', '1', '100', '00000000000', '40%暴击');
INSERT INTO `skill_book_info` VALUES ('280', '恶念波动', '恶', '80', '3', '100', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('281', '偷袭', '恶', '60', '1', '100', '00000000000', '必定暴击');
INSERT INTO `skill_book_info` VALUES ('319', '落井下石', '恶', '60', '1', '100', '00000000000', '30%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('320', '暗算', '恶', '60', '1', '101', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('322', '咬碎', '恶', '80', '1', '100', '00000000000', '20%弱化物防');
INSERT INTO `skill_book_info` VALUES ('323', '寻衅', '恶', '0', '2', '100', '00000000000', '使对方不能连续使用相同技能');
INSERT INTO `skill_book_info` VALUES ('326', '挑拨', '恶', '0', '2', '100', '00000000000', '使对方3回合不能使用变化技能');
INSERT INTO `skill_book_info` VALUES ('327', '精神切割', '超', '80', '1', '100', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('328', '心灵压迫', '超', '80', '1', '90', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('334', '精神干扰', '超', '95', '3', '100', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('335', '精神冲击', '超', '95', '3', '100', '00000000000', '造成物理伤害');
INSERT INTO `skill_book_info` VALUES ('336', '精神病', '超', '120', '3', '100', '00000000000', '连续攻击2~3回合');
INSERT INTO `skill_book_info` VALUES ('337', '预知未来', '超', '120', '3', '100', '00000000000', '2回合后攻击出现');
INSERT INTO `skill_book_info` VALUES ('338', '精神增压', '超', '140', '3', '90', '00000000000', '大幅弱化自身特攻');
INSERT INTO `skill_book_info` VALUES ('339', '催眠术', '超', '0', '2', '60', '00000000000', '催眠对方');
INSERT INTO `skill_book_info` VALUES ('340', '高速移动', '超', '0', '2', '101', '00000000000', '大幅强化速度');
INSERT INTO `skill_book_info` VALUES ('341', '栅栏', '超', '0', '2', '101', '00000000000', '大幅强化物防');
INSERT INTO `skill_book_info` VALUES ('342', '光之壁', '超', '0', '2', '101', '00000000000', '5回合特殊伤害减半');
INSERT INTO `skill_book_info` VALUES ('343', '反射盾', '超', '0', '2', '101', '00000000000', '5回合物理伤害减半');
INSERT INTO `skill_book_info` VALUES ('344', '超级健忘', '超', '0', '2', '101', '00000000000', '大幅强化特防');
INSERT INTO `skill_book_info` VALUES ('345', '封印', '超', '0', '2', '101', '00000000000', '对方不能使用我方会的技能');
INSERT INTO `skill_book_info` VALUES ('346', '冥想', '超', '0', '2', '101', '00000000000', '强化特攻，强化特防');
INSERT INTO `skill_book_info` VALUES ('347', '重力', '超', '0', '2', '101', '00000000000', '5回合浮游与飞行系能被地系技能打中');
INSERT INTO `skill_book_info` VALUES ('348', '欺骗空间', '超', '0', '2', '101', '00000000000', '5回合速度慢的先行动');
INSERT INTO `skill_book_info` VALUES ('349', '龙之尾', '龙', '60', '1', '90', '00000000000', '强制换人，对野生怪兽则结束战斗。');
INSERT INTO `skill_book_info` VALUES ('350', '龙之爪', '龙', '80', '1', '100', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('351', '龙之冲锋', '龙', '100', '1', '80', '00000000000', '20%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('352', '逆鳞', '龙', '120', '1', '100', '00000000000', '连续攻击2～3回合');
INSERT INTO `skill_book_info` VALUES ('354', '龙之吐息', '龙', '75', '3', '90', '00000000000', '30%麻痹');
INSERT INTO `skill_book_info` VALUES ('357', '龙星群', '龙', '140', '3', '90', '00000000000', '大幅弱化自身特攻');
INSERT INTO `skill_book_info` VALUES ('359', '龙之舞', '龙', '0', '2', '101', '00000000000', '强化物攻，强化速度');
INSERT INTO `skill_book_info` VALUES ('360', '螺旋球', '钢', '90', '1', '100', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('361', '子弹拳', '钢', '60', '1', '100', '00000000000', '先制+1');
INSERT INTO `skill_book_info` VALUES ('362', '钢尾', '钢', '70', '1', '90', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('365', '铁头槌', '钢', '80', '1', '100', '00000000000', '30%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('367', '彗星拳', '钢', '100', '1', '90', '00000000000', '30%强化物攻');
INSERT INTO `skill_book_info` VALUES ('368', '镜面射击', '钢', '80', '3', '90', '00000000000', '30%强化特攻');
INSERT INTO `skill_book_info` VALUES ('369', '光栅加农', '钢', '80', '3', '100', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('370', '破灭之愿', '钢', '130', '1', '100', '00000000000', '2回合后攻击出现');
INSERT INTO `skill_book_info` VALUES ('371', '变换齿轮', '钢', '0', '2', '101', '00000000000', '大幅强化速度');
INSERT INTO `skill_book_info` VALUES ('372', '铁壁', '钢', '0', '2', '101', '00000000000', '大幅强化物防');
INSERT INTO `skill_book_info` VALUES ('494', '终极针刺', '虫', '140', '1', '90', '00000000000', '下回合不能行动');
INSERT INTO `skill_book_info` VALUES ('496', '蜻蜓回转', '虫', '70', '1', '100', '00000000000', '攻击后交换巴迪出场');
INSERT INTO `skill_book_info` VALUES ('497', '十字剪', '虫', '90', '1', '100', '00000000000', '30%暴击');
INSERT INTO `skill_book_info` VALUES ('499', '百万角击', '虫', '120', '1', '85', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('500', '蜂涌', '虫', '120', '1', '95', '00000000000', '连续攻击2~3回合');
INSERT INTO `skill_book_info` VALUES ('501', '信号光线', '虫', '75', '3', '100', '00000000000', '30%强化特攻');
INSERT INTO `skill_book_info` VALUES ('502', '虫鸣', '虫', '90', '3', '100', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('503', '蜘蛛网', '虫', '0', '2', '100', '00000000000', '防止对方换人和逃跑');
INSERT INTO `skill_book_info` VALUES ('504', '萤火', '虫', '0', '2', '101', '00000000000', '大幅强化特攻');
INSERT INTO `skill_book_info` VALUES ('505', '蝶之舞', '虫', '0', '2', '101', '00000000000', '强化特攻，强化特防，强化速度');
INSERT INTO `skill_book_info` VALUES ('506', '丢石头', '岩', '30', '1', '90', '00000000000', '');
INSERT INTO `skill_book_info` VALUES ('508', '岩崩', '岩', '80', '1', '95', '00000000000', '30%使对方害怕');
INSERT INTO `skill_book_info` VALUES ('509', '石刃', '岩', '100', '1', '75', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('511', '岩石炮', '岩', '150', '1', '90', '00000000000', '下回合不能行动');
INSERT INTO `skill_book_info` VALUES ('512', '双刃头槌', '岩', '150', '1', '80', '00000000000', '1/3反伤');
INSERT INTO `skill_book_info` VALUES ('513', '原始力量', '岩', '90', '3', '100', '00000000000', '20%暴击');
INSERT INTO `skill_book_info` VALUES ('514', '力量宝石', '岩', '90', '3', '100', '00000000000', '30%强化特攻');
INSERT INTO `skill_book_info` VALUES ('515', '沙暴', '岩', '0', '2', '101', '00000000000', '5回合沙暴天气，地岩钢以外属性巴迪受到1/16伤害');
INSERT INTO `skill_book_info` VALUES ('516', '岩切', '岩', '0', '2', '101', '00000000000', '大幅强化速度');
INSERT INTO `skill_book_info` VALUES ('518', '地裂', '地', '0', '1', '30', '00000000000', '1击毙');
INSERT INTO `skill_book_info` VALUES ('520', '地震', '地', '100', '1', '100', '00000000000', '全体攻击');
INSERT INTO `skill_book_info` VALUES ('522', '大地之力', '地', '90', '3', '100', '00000000000', '30%弱化特防');
INSERT INTO `skill_book_info` VALUES ('523', '撒菱', '地', '0', '2', '101', '00000000000', '使上场巴迪受到1/8伤害');
INSERT INTO `skill_book_info` VALUES ('524', '剧毒之牙', '毒', '60', '1', '100', '00000000000', '20%中毒');
INSERT INTO `skill_book_info` VALUES ('526', '毒突', '毒', '80', '1', '100', '00000000000', '20%弱化物防');
INSERT INTO `skill_book_info` VALUES ('527', '粉尘射击', '毒', '120', '1', '80', '00000000000', '20%中毒');
INSERT INTO `skill_book_info` VALUES ('529', '淤泥爆弹', '毒', '90', '3', '100', '00000000000', '20%中毒');
INSERT INTO `skill_book_info` VALUES ('530', '毒粉', '毒', '0', '2', '75', '00000000000', '中毒');
INSERT INTO `skill_book_info` VALUES ('531', '剧毒', '毒', '0', '2', '85', '00000000000', '中毒');
INSERT INTO `skill_book_info` VALUES ('534', '香甜气息', '草', '0', '2', '100', '00000000001', '催眠对方，非战斗使用可引出野生巴迪');

-- ----------------------------
-- Table structure for `talent`
-- ----------------------------
DROP TABLE IF EXISTS `talent`;
CREATE TABLE `talent` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL COMMENT '特性名',
  `descript` varchar(255) DEFAULT NULL COMMENT '特性描述',
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `name_2` (`name`),
  KEY `name_3` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=120 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of talent
-- ----------------------------
INSERT INTO `talent` VALUES ('1', '威吓', '出场对放物理攻击降低1级');
INSERT INTO `talent` VALUES ('2', '石脑', '免疫反噬');
INSERT INTO `talent` VALUES ('3', '净体', '免疫弱化');
INSERT INTO `talent` VALUES ('4', '压力', '对方容易失误，20%闪避');
INSERT INTO `talent` VALUES ('5', '霸气', '20%使对方害怕');
INSERT INTO `talent` VALUES ('6', '暴雨', '永久雨天');
INSERT INTO `talent` VALUES ('7', '扬沙', '永久沙暴');
INSERT INTO `talent` VALUES ('8', '干旱', '永久晴天');
INSERT INTO `talent` VALUES ('9', '盔甲', '免疫暴击');
INSERT INTO `talent` VALUES ('10', '结实', '不受1击毙，满体力时有忍耐效果');
INSERT INTO `talent` VALUES ('12', '柔软', '免疫麻痹');
INSERT INTO `talent` VALUES ('13', '沙隐', '沙暴闪避提升');
INSERT INTO `talent` VALUES ('14', '雪隐', '冰雹闪避提升');
INSERT INTO `talent` VALUES ('15', '静电', '受到物理攻击时，30%麻痹对手');
INSERT INTO `talent` VALUES ('16', '食草', '草系无效，强化物攻');
INSERT INTO `talent` VALUES ('17', '蓄水', '水系无效，回复1/4体力');
INSERT INTO `talent` VALUES ('18', '引火', '火系无效，火系威力*1.5');
INSERT INTO `talent` VALUES ('19', '电气引擎', '电系无效，强化速度');
INSERT INTO `talent` VALUES ('21', '复眼', '命中提高30%');
INSERT INTO `talent` VALUES ('22', '失眠', '不会睡眠');
INSERT INTO `talent` VALUES ('23', '免疫', '不会中毒');
INSERT INTO `talent` VALUES ('25', '磷粉', '不吃特效');
INSERT INTO `talent` VALUES ('28', '咬脚', '禁止逃跑，换人');
INSERT INTO `talent` VALUES ('30', '蛇皮', '受到物理攻击时，反弹');
INSERT INTO `talent` VALUES ('35', '蜕皮', '30%解除异常');
INSERT INTO `talent` VALUES ('36', '根性', '异常时物攻*1.5，烧伤不减物攻');
INSERT INTO `talent` VALUES ('37', '神秘鳞片', '异常时物防*1.5');
INSERT INTO `talent` VALUES ('39', '茂盛', '体力1/3时草系威力*1.5');
INSERT INTO `talent` VALUES ('40', '猛火', '体力1/3时火系威力*1.5');
INSERT INTO `talent` VALUES ('41', '激流', '体力1/3时水系威力*1.5');
INSERT INTO `talent` VALUES ('42', '虫族报警', '体力1/3时虫系威力*1.5');
INSERT INTO `talent` VALUES ('44', '浮游', '不受地属性攻击');
INSERT INTO `talent` VALUES ('45', '孢子', '受到物理攻击时，30%中毒');
INSERT INTO `talent` VALUES ('46', '自然恢复', '退场解除异常');
INSERT INTO `talent` VALUES ('48', '轻快', '雨天速度*2');
INSERT INTO `talent` VALUES ('49', '叶绿素', '晴天速度*2');
INSERT INTO `talent` VALUES ('50', '挖沙', '沙暴速度*2，不受沙暴伤害');
INSERT INTO `talent` VALUES ('51', '换装', '复制特性');
INSERT INTO `talent` VALUES ('52', '精神力', '不害怕');
INSERT INTO `talent` VALUES ('56', '雨盘', '雨天回复1/16');
INSERT INTO `talent` VALUES ('57', '加速', '每回合结束强化速度');
INSERT INTO `talent` VALUES ('58', '肥脂肪', '冰火属性伤害减半');
INSERT INTO `talent` VALUES ('60', '火焰体', '不冰冻，受到物理攻击时，30%烧伤');
INSERT INTO `talent` VALUES ('61', '逃走', '必定可以交换和逃跑');
INSERT INTO `talent` VALUES ('62', '锐利目光', '技能必中');
INSERT INTO `talent` VALUES ('64', '不屈', '害怕后强化速度');
INSERT INTO `talent` VALUES ('65', '正义心', '被恶属性攻击，强化物攻');
INSERT INTO `talent` VALUES ('71', '毒疗', '中毒时回合结束恢复1/16');
INSERT INTO `talent` VALUES ('73', '湿润身躯', '雨天不异常，解除异常');
INSERT INTO `talent` VALUES ('74', '太阳力量', '晴天特攻*1.5，每回合结束损失1/8体力');
INSERT INTO `talent` VALUES ('75', '飞毛腿', '异常时候速度*1.5，麻痹不减速度');
INSERT INTO `talent` VALUES ('76', '强运', '提升暴击几率');
INSERT INTO `talent` VALUES ('77', '魔法防御', '只收攻击技能伤害');
INSERT INTO `talent` VALUES ('80', '技师', '威力小于60的技能伤害*1.5');
INSERT INTO `talent` VALUES ('81', '叶片防御', '晴天不异常，解除异常');
INSERT INTO `talent` VALUES ('83', '有色眼镜', '伤害无修正时，威力翻倍');
INSERT INTO `talent` VALUES ('84', '胆量', '可以击中鬼属性巴迪');
INSERT INTO `talent` VALUES ('85', '冰冻身体', '受到物理攻击时，30%冰冻');
INSERT INTO `talent` VALUES ('86', '舍身', '反噬类技能威力*1.2，不会因反噬体力归0');
INSERT INTO `talent` VALUES ('87', '花之礼', '晴天强化特攻强化特防');
INSERT INTO `talent` VALUES ('88', '给力', '伤害*1.3，进攻特效不发动');
INSERT INTO `talent` VALUES ('89', '圣斗士', '见过的技能无效');
INSERT INTO `talent` VALUES ('93', '毒手', '物理攻击30%使中毒');
INSERT INTO `talent` VALUES ('94', '再生', '退场回复1/3体力');
INSERT INTO `talent` VALUES ('96', '分析', '后手威力翻倍');
INSERT INTO `talent` VALUES ('98', '变身', '变成对方');
INSERT INTO `talent` VALUES ('99', '穿透', '无视光墙，反射盾');
INSERT INTO `talent` VALUES ('101', '魔装反射', '反弹对方变化技能');
INSERT INTO `talent` VALUES ('102', '沙之力', '沙暴中地岩钢技能威力*1.5');
INSERT INTO `talent` VALUES ('107', '天空皮肤', '普系边飞行，且威力*130%');
INSERT INTO `talent` VALUES ('108', '黑暗气息', '恶系威力*1.5');
INSERT INTO `talent` VALUES ('110', '疾风之翼', '飞行系技能先至+1');
INSERT INTO `talent` VALUES ('113', '冰冻皮肤', '普系边冰，且威力*130%');
INSERT INTO `talent` VALUES ('114', '超级炮台', '伤害*1.2，速度*1.2');
INSERT INTO `talent` VALUES ('115', '暴力', '物攻*2');
INSERT INTO `talent` VALUES ('116', '暴雪', '永久冰雹');
INSERT INTO `talent` VALUES ('117', '天然', '所有能力变化无效');
INSERT INTO `talent` VALUES ('119', '滑雪', '冰雹天气速度翻倍');

-- ----------------------------
-- Table structure for `talent_book_effect`
-- ----------------------------
DROP TABLE IF EXISTS `talent_book_effect`;
CREATE TABLE `talent_book_effect` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `talent` varchar(255) NOT NULL COMMENT '特性',
  `step` varchar(255) DEFAULT '战斗' COMMENT '触发时机',
  `effect` varchar(255) DEFAULT '霸气' COMMENT '特效',
  `probability` int(11) DEFAULT '100' COMMENT '概率',
  `agent` tinyint(4) DEFAULT '0' COMMENT '作用对象：0对方，1自己，2全场，3天气',
  PRIMARY KEY (`id`),
  KEY `step_name1` (`step`),
  KEY `effect_name1` (`effect`),
  KEY `skill_name1` (`talent`),
  CONSTRAINT `talent_book_effect_ibfk_3` FOREIGN KEY (`step`) REFERENCES `battle_step` (`step`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `talent_book_effect_ibfk_1` FOREIGN KEY (`talent`) REFERENCES `talent` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `talent_book_effect_ibfk_2` FOREIGN KEY (`effect`) REFERENCES `ex_effect` (`name`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=380 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of talent_book_effect
-- ----------------------------
INSERT INTO `talent_book_effect` VALUES ('254', '威吓', '换人', '弱化物攻', '100', '0');
INSERT INTO `talent_book_effect` VALUES ('255', '石脑', '战斗', '免疫反噬', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('256', '净体', '战斗', '免疫弱化', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('257', '压力', '战斗', '提高闪避', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('258', '霸气', '战斗', '霸气', '20', '0');
INSERT INTO `talent_book_effect` VALUES ('259', '暴雨', '换人', '永久雨天', '100', '3');
INSERT INTO `talent_book_effect` VALUES ('260', '暴雪', '换人', '永久冰雹', '100', '3');
INSERT INTO `talent_book_effect` VALUES ('261', '干旱', '换人', '永久晴天', '100', '3');
INSERT INTO `talent_book_effect` VALUES ('262', '扬沙', '换人', '永久沙暴', '100', '3');
INSERT INTO `talent_book_effect` VALUES ('263', '盔甲', '战斗', '免疫暴击', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('264', '结实', '战斗', '免疫1击毙', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('265', '柔软', '战斗', '免疫麻痹', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('266', '食草', '战斗', '免疫草系', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('267', '食草', '战斗', '免疫特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('268', '蓄水', '战斗', '免疫水系', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('269', '蓄水', '战斗', '免疫特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('270', '引火', '战斗', '免疫火系', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('271', '引火', '战斗', '火系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('272', '电气引擎', '战斗', '免疫电系', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('273', '电气引擎', '战斗', '强化速度', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('274', '复眼', '战斗', '提高命中', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('275', '失眠', '战斗', '免疫催眠', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('276', '免疫', '战斗', '免疫中毒', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('277', '磷粉', '战斗', '免疫特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('278', '咬脚', '战斗', '留场', '100', '0');
INSERT INTO `talent_book_effect` VALUES ('279', '蛇皮', '战斗', '受到物理攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('280', '蜕皮', '战斗', '解除异常', '30', '1');
INSERT INTO `talent_book_effect` VALUES ('283', '茂盛', '战斗', '重伤时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('284', '茂盛', '战斗', '地系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('285', '猛火', '战斗', '重伤时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('286', '猛火', '战斗', '火系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('287', '激流', '战斗', '重伤时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('288', '激流', '战斗', '水系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('289', '虫族报警', '战斗', '重伤时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('290', '虫族报警', '战斗', '虫系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('291', '浮游', '战斗', '免疫地系', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('292', '孢子', '战斗', '受到物理攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('293', '孢子', '战斗', '中毒', '30', '0');
INSERT INTO `talent_book_effect` VALUES ('294', '自然恢复', '换人', '解除异常', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('295', '轻快', '战斗', '雨天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('296', '轻快', '战斗', '速度翻倍', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('297', '叶绿素', '战斗', '晴天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('298', '叶绿素', '战斗', '速度翻倍', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('299', '挖沙', '战斗', '沙暴特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('300', '挖沙', '战斗', '速度翻倍', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('301', '换装', '换人', '复制特性', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('302', '精神力', '战斗', '免疫霸气', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('303', '雨盘', '回合结束', '雨天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('304', '雨盘', '回合结束', '微量回复', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('305', '加速', '回合结束', '强化速度', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('306', '肥脂肪', '战斗', '冰火属性伤害减半', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('307', '火焰体', '战斗', '免疫冰封', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('308', '火焰体', '战斗', '受到物理攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('309', '火焰体', '战斗', '烧伤', '30', '0');
INSERT INTO `talent_book_effect` VALUES ('310', '逃走', '战斗', '免疫留场', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('311', '锐利目光', '战斗', '技能必中', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('312', '不屈', '战斗', '害怕时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('313', '不屈', '战斗', '强化速度', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('314', '正义心', '战斗', '受恶系攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('315', '正义心', '战斗', '强化物攻', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('316', '毒疗', '战斗', '中毒发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('317', '毒疗', '回合结束', '微量回复', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('318', '湿润身躯', '战斗', '雨天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('319', '湿润身躯', '战斗', '解除异常', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('320', '叶片防御', '战斗', '晴天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('321', '叶片防御', '战斗', '解除异常', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('322', '太阳力量', '战斗', '晴天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('323', '太阳力量', '战斗', '强攻', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('324', '太阳力量', '战斗', '强体', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('325', '飞毛腿', '战斗', '自身异常时发动特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('326', '飞毛腿', '战斗', '强速', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('327', '神秘鳞片', '战斗', '自身异常时发动特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('328', '神秘鳞片', '战斗', '强体', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('329', '根性', '战斗', '自身异常时发动特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('330', '根性', '战斗', '强力', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('331', '强运', '战斗', '提高暴击率', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('332', '魔法防御', '战斗', '只受攻击伤害', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('333', '技师', '战斗', '威力小于60的技能发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('334', '技师', '战斗', '伤害提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('335', '有色眼镜', '战斗', '效果低于1倍时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('337', '有色眼镜', '战斗', '威力翻倍', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('338', '胆量', '战斗', '胆量', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('339', '冰冻身体', '战斗', '受到物理攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('340', '冰冻身体', '战斗', '冰封', '30', '0');
INSERT INTO `talent_book_effect` VALUES ('341', '舍身', '战斗', '无限反噬', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('343', '花之礼', '战斗', '晴天特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('344', '花之礼', '战斗', '强化特攻', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('345', '花之礼', '战斗', '强化特防', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('346', '给力', '战斗', '伤害*1.3', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('347', '给力', '战斗', '免疫特效', '100', '2');
INSERT INTO `talent_book_effect` VALUES ('348', '圣斗士', '战斗', '圣斗士', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('349', '毒手', '战斗', '物理技能发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('350', '毒手', '战斗', '中毒', '30', '0');
INSERT INTO `talent_book_effect` VALUES ('351', '再生', '换人', '回复1/3体力', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('352', '分析', '战斗', '后攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('353', '分析', '战斗', '威力翻倍', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('354', '变身', '战斗', '变身', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('355', '穿透', '战斗', '穿透', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('356', '魔装反射', '战斗', '反弹变化技能', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('357', '沙之力', '战斗', '沙暴特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('358', '沙之力', '战斗', '地系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('359', '沙之力', '战斗', '岩系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('360', '沙之力', '战斗', '钢系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('361', '天空皮肤', '战斗', '普系边冰，且威力*130%', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('362', '黑暗气息', '战斗', '恶系提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('363', '疾风之翼', '战斗', '飞行技能特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('364', '疾风之翼', '战斗', '先手', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('365', '冰冻皮肤', '战斗', '普系边冰，且威力*130%', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('366', '超级炮台', '战斗', '速度*1.2', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('367', '超级炮台', '战斗', '伤害*1.2', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('368', '暴力', '战斗', '物攻*2', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('369', '天然', '战斗', '无视对方能力变化', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('370', '沙隐', '战斗', '沙暴特效', '100', '0');
INSERT INTO `talent_book_effect` VALUES ('371', '沙隐', '战斗', '闪避', '20', '1');
INSERT INTO `talent_book_effect` VALUES ('372', '雪隐', '战斗', '冰雹特效', '100', '0');
INSERT INTO `talent_book_effect` VALUES ('373', '雪隐', '战斗', '闪避', '20', '1');
INSERT INTO `talent_book_effect` VALUES ('374', '静电', '战斗', '受到物理攻击时发动', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('375', '静电', '战斗', '麻痹', '30', '0');
INSERT INTO `talent_book_effect` VALUES ('376', '蛇皮', '战斗', '反噬', '100', '0');
INSERT INTO `talent_book_effect` VALUES ('377', '舍身', '战斗', '反噬技能提升', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('378', '滑雪', '战斗', '冰雹特效', '100', '1');
INSERT INTO `talent_book_effect` VALUES ('379', '滑雪', '战斗', '速度翻倍', '100', '1');
