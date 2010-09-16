CREATE TABLE History (
Date TEXT UNIQUE ON CONFLICT ROLLBACK NOT NULL ON CONFLICT ROLLBACK PRIMARY KEY ON CONFLICT ROLLBACK ,
Series TEXT NOT NULL ON CONFLICT ROLLBACK ,
Number INTEGER NOT NULL ON CONFLICT ROLLBACK
);
CREATE TRIGGER AddSeriesInfo AFTER INSERT ON History WHEN
(
	 NOT EXISTS (SELECT * FROM SeriesInfo WHERE Title = new.Series)
)
BEGIN
	INSERT INTO SeriesInfo (Title,Current,Date,StartDate) 
	VALUES(NEW.Series,NEW.Number,NEW.Date,NEW.Date);
END;
CREATE TRIGGER OnSeriesFinish AFTER INSERT ON History WHEN
(
	EXISTS (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series) AND
	(NEW.Number = (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series))
)
BEGIN
	UPDATE SeriesInfo 
		SET Finished = 1, EndDate = New.Date
  	WHERE Title = new.Series;
END;
CREATE TRIGGER UpdateSeriesInfo AFTER INSERT ON History WHEN
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	(NEW.Number > (SELECT Current FROM SeriesInfo WHERE Title = NEW.Series) )
)
BEGIN
	UPDATE SeriesInfo 
		SET Current = new.Number, Date = NEW.Date
  	WHERE Title = new.Series;
END;
CREATE TRIGGER ValidateAddition BEFORE INSERT ON History
BEGIN
SELECT CASE
		 WHEN ( EXISTS(SELECT Date
			FROM   History
			WHERE  (Series = new.Series
					AND Number = new.Number )
			)
			AND (SELECT max(Date)
					FROM  history
					WHERE ( Series = new.Series AND Number = new.Number )
				) >= Datetime(new.Date, '-7 day') 
		) THEN
			Raise(abort, 'Just Added')
		END; 
END;
CREATE TRIGGER ValidateDate BEFORE INSERT ON History
BEGIN
	SELECT CASE
		WHEN (  
			length(NEW.Date) != 20 AND NOT(like('____-__-__ __:__:__',NEW.Date))
		)
		THEN RAISE(ABORT,'Date has to be in form yyyy-mm-dd HH:MM:SS')
	END;
END;
CREATE TABLE SeriesInfo (
Title TEXT PRIMARY KEY ON CONFLICT ROLLBACK UNIQUE ON CONFLICT ROLLBACK NOT NULL ON CONFLICT ROLLBACK ,
Current INTEGER NOT NULL ON CONFLICT ROLLBACK ,
Date Text NOT NULL ON CONFLICT ROLLBACK,
StartDate Text NOT NULL ON CONFLICT ROLLBACK,
EndDate Text,
Total INTEGER,
Finished INTEGER NOT NULL ON CONFLICT ROLLBACK DEFAULT '0' ,
Id INTEGER ,
Updated INTEGER NOT NULL ON CONFLICT ROLLBACK DEFAULT '0' ,
Skip INTEGER NOT NULL ON CONFLICT ROLLBACK DEFAULT '0' );
