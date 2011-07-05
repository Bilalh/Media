-- TODO MAKE NEW VERSION

CREATE TABLE History (
Date TEXT UNIQUE ON CONFLICT ROLLBACK NOT NULL ON CONFLICT ROLLBACK PRIMARY KEY ON CONFLICT ROLLBACK ,
Series TEXT NOT NULL ON CONFLICT ROLLBACK ,
Number INTEGER NOT NULL ON CONFLICT ROLLBACK
);
CREATE TRIGGER A01_OnSeriesFinished AFTER INSERT ON History WHEN
(
	EXISTS (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Rewatching FROM SeriesInfo WHERE Title = NEW.Series) = 0 ) AND
	(NEW.Number = (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series))
)
BEGIN
	UPDATE SeriesInfo 
		SET Finished = 1, EndDate = New.Date
  	WHERE Title = new.Series;
	INSERT INTO Watched(Series, DateStarted, DateFinished) 
		VALUES(NEW.Series,(SELECT StartDate FROM SeriesInfo WHERE Title = new.Series) , new.Date);
END;
CREATE TRIGGER A11_OnSeriesRewatch AFTER INSERT ON History WHEN
-- has to before OnSeriesRewatchFinish
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Rewatching FROM SeriesInfo WHERE Title = NEW.Series) = 0 ) AND
	( (SELECT Finished FROM SeriesInfo WHERE Title = NEW.Series) = 1 ) AND	(NEW.Number >=1  )
)
BEGIN
	UPDATE SeriesInfo 
		SET Rewatching = 1,
		RewatchingStart = NEW.Date
  	WHERE Title = new.Series;
END;
CREATE TRIGGER A22_SetUpdated AFTER INSERT ON History WHEN
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Rewatching FROM SeriesInfo WHERE Title = NEW.Series) = 0 ) 
)
BEGIN
	UPDATE SeriesInfo 
		SET Updated = 0
  	WHERE Title = new.Series;
END;
CREATE TRIGGER A41_UpdateSeriesInfoUpdatesOnRewatch AFTER INSERT ON History WHEN
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Rewatching FROM SeriesInfo WHERE Title = NEW.Series) = 1 ) AND	(NEW.Number > (SELECT RewatchingCurrent FROM SeriesInfo WHERE Title = NEW.Series) )
)
BEGIN
	UPDATE SeriesInfo 
		SET RewatchingCurrent = new.Number, RewatchingDate = NEW.Date
  	WHERE Title = new.Series;
END;
CREATE TRIGGER A45_OnSeriesRewatchFinish AFTER INSERT ON History WHEN
-- so that it will executed after onSeriesFinished and setUpdated
(
	EXISTS (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Rewatching FROM SeriesInfo WHERE Title = NEW.Series) = 1 ) AND
	(NEW.Number = (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series))
)
BEGIN

	INSERT into Watched(Series,DateStarted, DateFinished)
		VALUES(new.Series,(SELECT RewatchingStart FROM SeriesInfo WHERE Title = new.Series) ,new.Date);	
	UPDATE SeriesInfo 
		SET 
			Rewatching        = 0,
			RewatchingCurrent = 0,
			RewatchingStart   = NULL,
			RewatchingDate    = NULL
  	WHERE Title = new.Series;

END;
CREATE TRIGGER AddSeriesInfo AFTER INSERT ON History WHEN
(
	 NOT EXISTS (SELECT * FROM SeriesInfo WHERE Title = new.Series)
)
BEGIN
	INSERT INTO SeriesInfo (Title,Current,Date,StartDate) 
	VALUES(NEW.Series,NEW.Number,NEW.Date,NEW.Date);
END;
CREATE TRIGGER UpdateSeriesInfoUpdates AFTER INSERT ON History WHEN
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Rewatching FROM SeriesInfo WHERE Title = NEW.Series) = 0 ) AND
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
CREATE TRIGGER ValidateEpNum BEFORE INSERT ON History WHEN
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	(NEW.Number > (SELECT Current FROM SeriesInfo WHERE Title = NEW.Series) )
)
BEGIN
	SELECT CASE
		WHEN (Select Date FROM SeriesInfo WHERE Title = NEW.Series) >= NEW.Date THEN
			 RAISE(ABORT,'Newer ep can not be watched before an older ep ')
	END;
END;
CREATE TRIGGER ValidateTitle BEFORE INSERT ON History
BEGIN
	SELECT CASE
		WHEN (  
			NOT EXISTS (SELECT * FROM SeriesInfo WHERE Title = new.Series) AND
			EXISTS (SELECT * FROM SeriesInfo WHERE Title = new.Series collate nocase)
		)
		THEN RAISE(ABORT,'Malformed Series title')
	END;
END;
CREATE TABLE OldSeries (
Title TEXT PRIMARY KEY UNIQUE ,
Total INTEGER NOT NULL );
CREATE TABLE "SeriesInfo" (
"Title" TEXT PRIMARY KEY NOT NULL DEFAULT NULL,
"Current" INTEGER NOT NULL DEFAULT NULL,
"Date" Text NOT NULL DEFAULT NULL,
"StartDate" Text NOT NULL DEFAULT NULL,
"EndDate" Text DEFAULT NULL,
"Total" INTEGER DEFAULT NULL,
"Finished" INTEGER NOT NULL DEFAULT '0',
"Id" INTEGER DEFAULT NULL,
"Updated" INTEGER NOT NULL DEFAULT '0',
"Skip" INTEGER NOT NULL DEFAULT '0',
"Score" INTEGER DEFAULT NULL,
"Dropped" INTEGER NOT NULL DEFAULT '0',
"Rewatching" INTEGER NOT NULL DEFAULT '0',
"RewatchingCurrent" INTEGER NOT NULL DEFAULT '0',
"RewatchingDate" TEXT DEFAULT NULL,
"RewatchingStart" TEXT DEFAULT NULL);
CREATE TABLE "Watched" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL DEFAULT NULL,
"Series" TEXT NOT NULL DEFAULT NULL,
"DateStarted" TEXT NOT NULL DEFAULT NULL,
"DateFinished" TEXT NOT NULL DEFAULT NULL);

CREATE VIEW DroppedSeries As

Select 
	si.Title as Series,
	si.Current,
	si.Total,
	si.StartDate,
	si.Date

From SeriesInfo si

Where 
	si.Finished = 0 AND
	si.Dropped  = 1

Order by si.Title;
CREATE VIEW FinishedSeries As

Select 
si.Title as Series,
si.Total,
si.StartDate,
si.EndDate

From SeriesInfo si

Where si.Finished = 1
Order by si.Title;
CREATE VIEW OngoingSeries As

Select 
	si.Title as Series,
	si.Current,
	si.Total,
	si.Date,
	si.StartDate

From SeriesInfo si

Where 
	si.Finished = 0 AND
	si.Dropped  = 0

Order by si.Title;
CREATE VIEW Recent As
 SELECT 
	Title, Current, Total, Date, Finished, 
	Rewatching, RewatchingCurrent, RewatchingDate FROM SeriesInfo  
 WHERE( 
		strftime('%s',Date) > strftime('%%s', 'now','-14 day','localtime') 
	)OR( 
		Rewatching = 1 AND 
		strftime('%s',RewatchingDate) > strftime('%s', 'now','-14 day','localtime') 
	)

order by date desc;
CREATE VIEW RewatchingOngoingSeries As

Select 
	si.Title as Series,
	si.RewatchingCurrent,
	si.Total,
	si.RewatchingDate as Date,
	si.RewatchingStart as StartDate

From SeriesInfo si

Where 
	si.Rewatching = 1 OR 
	(	-- It was watched in the past 
		EXISTS(Select * From OldSeries os Where os.Title = si.Title) AND 
		EXISTS(Select * From OngoingSeries og Where og.Series = si.Title)  
	)

Order by si.Title;
CREATE VIEW SeriesData as
SELECT 

	si.Title, 
	(
		SELECT CASE 
			WHEN si.Rewatching = '1'THEN si.RewatchingCurrent 
			ELSE si.Current 
		END
	) as Current, 
	
	si.Total,
	(
		SELECT CASE 
			WHEN si.Rewatching = '1'THEN si.RewatchingDate 
			ELSE si.Date 
		END
	) as Date, 

	(
		SELECT CASE 
			WHEN si.Rewatching = '1'THEN 0 
			ELSE si.Finished 
		END
	) as Finished, 

	(
		SELECT CASE WHEN 
			Exists(Select * From OldSeries os Where os.Title=si.Title ) OR
			si.Rewatching = '1' 
		THEN '1' ELSE '0' 
		END
	) as Rewatching,
	
	si.Skip,
	si.dropped,
	si.Score,
	(
		SELECT CASE 
			WHEN si.Rewatching = '1'THEN si.RewatchingStart
			ELSE si.StartDate 
		END
	) as StartDate,

	(
		SELECT CASE 
			WHEN si.Rewatching = '1'THEN NULL
			ELSE si.EndDate 
		END
	) as EndDate,
	
	si.Updated,
	si.Id

FROM SeriesInfo si
ORDER BY si.Title;
CREATE VIEW WatchedSeriesDates As
Select 
	w.Series,
	si.Total,
	w.DateStarted,
	w.DateFinished,
	round(julianday(w.DateFinished) - julianday(w.DateStarted)) as Days

From Watched w Join
	SeriesInfo si On si.Title = w.Series

Order by w.Series;
