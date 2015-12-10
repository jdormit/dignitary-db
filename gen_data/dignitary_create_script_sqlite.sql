DROP TABLE IF EXISTS `characters` ;

CREATE TABLE IF NOT EXISTS `characters` (
  `id` INT NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  `title` VARCHAR(45) NULL,
  PRIMARY KEY (`id`)  );



-- -----------------------------------------------------
-- Table `region`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `region` ;

CREATE TABLE IF NOT EXISTS `region` (
  `id` INT NOT NULL ,
  `name` VARCHAR(45) NULL ,
  `ruling_principality_id` INT NOT NULL ,
  PRIMARY KEY (`id`)  ,
  CONSTRAINT `fk_region_principality1`
    FOREIGN KEY (`ruling_principality_id`)
    REFERENCES `principality` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);



-- -----------------------------------------------------
-- Table `settlement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `settlement` ;

CREATE TABLE IF NOT EXISTS `settlement` (
  `id` INT NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  `size` INT NOT NULL ,
  `region_id` INT NOT NULL ,
  PRIMARY KEY (`id`)  ,
  CONSTRAINT `fk_settlement_region1`
    FOREIGN KEY (`region_id`)
    REFERENCES `region` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table `principality`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `principality` ;

CREATE TABLE IF NOT EXISTS `principality` (
  `id` INT NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  `ruling_faction_id` INT NOT NULL ,
 -- `capital_city_id` INT NOT NULL ,
  PRIMARY KEY (`id`)  ,
  CONSTRAINT `fk_principality_faction1`
    FOREIGN KEY (`ruling_faction_id`)
    REFERENCES `faction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION );
  -- CONSTRAINT `fk_principality_settlement1` will be created dynamically when simulation is generated
    -- FOREIGN KEY (`capital_city_id`)
    -- REFERENCES `settlement` (`id`)
    -- ON DELETE NO ACTION
    -- ON UPDATE NO ACTION);



-- -----------------------------------------------------
-- Table `faction`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `faction` ;

CREATE TABLE IF NOT EXISTS `faction` (
  `id` INT NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  `type` VARCHAR(45) NULL ,
  `goals` VARCHAR(100) NULL ,
  PRIMARY KEY (`id`) );
  -- FOREIGN KEY home_principality (principality.id) is added dynamically during table population



-- -----------------------------------------------------
-- Table `characters_relationship`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `characters_relationship` ;

CREATE TABLE IF NOT EXISTS `characters_relationship` (
  `rel_value` INT NULL ,
  `char_id` INT NOT NULL ,
  `char_other_id` INT NOT NULL ,
  PRIMARY KEY (`char_other_id`, `char_id`)  ,
  CONSTRAINT `fk_characters_relationship_characters1`
    FOREIGN KEY (`char_id`)
    REFERENCES `characters` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_characters_relationship_characters2`
    FOREIGN KEY (`char_other_id`)
    REFERENCES `characters` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);



-- -----------------------------------------------------
-- Table `characters_faction`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `characters_faction` ;

CREATE TABLE IF NOT EXISTS `characters_faction` (
  `faction_id` INT NOT NULL ,
  `rank` INT NULL ,
  `characters_id` INT NOT NULL ,
  PRIMARY KEY (`faction_id`, `characters_id`)  ,
  CONSTRAINT `fk_faction_has_character_faction1`
    FOREIGN KEY (`faction_id`)
    REFERENCES `faction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_characters_faction_characters1`
    FOREIGN KEY (`characters_id`)
    REFERENCES `characters` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);



-- -----------------------------------------------------
-- Table `faction_relationship`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `faction_relationship` ;

CREATE TABLE IF NOT EXISTS `faction_relationship` (
  `faction_id` INT NOT NULL ,
  `faction_id_other` INT NOT NULL ,
  `rel_value` INT NULL ,
  PRIMARY KEY (`faction_id`, `faction_id_other`)  ,
  CONSTRAINT `fk_faction_has_faction_faction1`
    FOREIGN KEY (`faction_id`)
    REFERENCES `faction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_faction_has_faction_faction2`
    FOREIGN KEY (`faction_id_other`)
    REFERENCES `faction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);



-- -----------------------------------------------------
-- Table `resource`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `resource` ;

CREATE TABLE IF NOT EXISTS `resource` (
  `id` INT NOT NULL ,
  `name` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id`)  );



-- -----------------------------------------------------
-- Table `region_resource`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `region_resource` ;

CREATE TABLE IF NOT EXISTS `region_resource` (
  `region_id` INT NOT NULL ,
  `resource_id` INT NOT NULL ,
  `amount` INT NULL ,
  PRIMARY KEY (`region_id`, `resource_id`)  ,
  CONSTRAINT `fk_resource_has_region_resource1`
    FOREIGN KEY (`resource_id`)
    REFERENCES `resource` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_resource_has_region_region1`
    FOREIGN KEY (`region_id`)
    REFERENCES `region` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);



-- -----------------------------------------------------
-- Table `resource_market_value`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `resource_market_value` ;

CREATE TABLE IF NOT EXISTS `resource_market_value` (
  `settlement_id` INT NOT NULL ,
  `resource_id` INT NOT NULL ,
  `market_value` FLOAT NOT NULL ,
  PRIMARY KEY (`settlement_id`, `resource_id`)  ,
  CONSTRAINT `fk_settlement_has_resource_settlement1`
    FOREIGN KEY (`settlement_id`)
    REFERENCES `settlement` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_settlement_has_resource_resource1`
    FOREIGN KEY (`resource_id`)
    REFERENCES `resource` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);