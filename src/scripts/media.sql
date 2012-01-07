CREATE TABLE "History" (
"Date" TEXT UNIQUE ON CONFLICT ROLLBACK NOT NULL ON CONFLICT ROLLBACK PRIMARY KEY ON CONFLICT ROLLBACK CHECK (like('____-__-__ __:__:__',Date)),
"Series" TEXT NOT NULL ON CONFLICT ROLLBACK CHECK (length(Series) >=1),
"Number" INTEGER NOT NULL ON CONFLICT ROLLBACK CHECK (Number >= 0)
);
CREATE TABLE "MalData" (
"Id" INTEGER PRIMARY KEY UNIQUE NOT NULL,
"Status" TEXT,
"ImageUrl" TEXT,
"Type" TEXT,
"ListedAnimeId" TEXT,
"Classification" TEXT,
"MembersScore" INTEGER,
"MembersCount" INTEGER NOT NULL DEFAULT 0,
"Rank" INTEGER NOT NULL DEFAULT 0,
"FavoritedCount" INTEGER NOT NULL DEFAULT 0,
"Title" TEXT NOT NULL UNIQUE,
"PopularityRank" INTEGER,
"Episodes" INTEGER,
"Synopsis" TEXT,
"Year" INTEGER,
"StartAirDate" TEXT,
"EndAirDate" TEXT,
"English" TEXT,
"Japanese" TEXT
);
CREATE TABLE "MalGenres" (
"Id" INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE,
"SeriesId" INTEGER NOT NULL,
"Genre" TEXT NOT NULL
);
CREATE TABLE "MalRelation" (
"Id" INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE,
"SeriesId" TEXT NOT NULL,
"RelationType" TEXT NOT NULL,
"RelationId" INTEGER NOT NULL,
"RelationUrl" TEXT NOT NULL,
"RelationTitle" TEXT NOT NULL
);
CREATE TABLE "MalSynonyms" (
"Id" INTEGER PRIMARY KEY ASC AUTOINCREMENT UNIQUE NOT NULL,
"SeriesId" INTEGER NOT NULL,
"Synonym" TEXT NOT NULL
);
CREATE TABLE "MalTags" (
"Id" INTEGER PRIMARY KEY ASC AUTOINCREMENT NOT NULL UNIQUE,
"SeriesId" INTEGER NOT NULL,
"Tag" TEXT NOT NULL
);
CREATE TABLE "OldSeries" (
"Title" TEXT PRIMARY KEY UNIQUE NOT NULL,
"Current" INTEGER NOT NULL DEFAULT 1,
"Total" INTEGER,
"Score" INTEGER,
"Id" INTEGER,
"StartDate" TEXT,
"EndDate" TEXT,
"Finished" INTEGER NOT NULL DEFAULT 0,
"Updated" INTEGER NOT NULL DEFAULT 0,
"Skip" INTEGER NOT NULL DEFAULT 0,
"Dropped" INTEGER NOT NULL DEFAULT 0,
"Date" TEXT
);
CREATE TABLE "SeriesInfo" (
"Title" TEXT PRIMARY KEY NOT NULL DEFAULT NULL UNIQUE CHECK (Length(Title) >= 1),
"Current" INTEGER NOT NULL DEFAULT NULL CHECK ((Current >= 0) or (Total is not NULL and Current <= Total)),
"Date" Text NOT NULL DEFAULT NULL CHECK (like('____-__-__ __:__:__',Date)),
"StartDate" Text NOT NULL DEFAULT NULL CHECK (like('____-__-__ __:__:__',StartDate)),
"EndDate" Text DEFAULT NULL CHECK (EndDate is NULL or like('____-__-__ __:__:__',EndDate)),
"Total" INTEGER DEFAULT NULL CHECK (Total is NULL or Total >0 ),
"Finished" INTEGER NOT NULL DEFAULT '0' CHECK (Finished in (0,1)),
"Id" INTEGER DEFAULT NULL CHECK (Id is NULL or Id >= 0),
"Updated" INTEGER NOT NULL DEFAULT '0' CHECK (Updated in (0,1)),
"Skip" INTEGER NOT NULL DEFAULT '0' CHECK (Skip in (0,1)),
"Score" INTEGER DEFAULT NULL CHECK (Score > 0 and Score <= 10 ),
"Dropped" INTEGER NOT NULL DEFAULT '0' CHECK (Dropped in(0,1)),
"Rewatching" INTEGER NOT NULL DEFAULT '0' CHECK (Rewatching in (0,1)),
"RewatchingCurrent" INTEGER NOT NULL DEFAULT '0',
"RewatchingDate" TEXT DEFAULT NULL CHECK (RewatchingDate is NULL or like('____-__-__ __:__:__',RewatchingDate)),
"RewatchingStart" TEXT DEFAULT NULL CHECK (RewatchingStart is NULL or like('____-__-__ __:__:__',RewatchingStart)),
"IsAnime" INTEGER CHECK (ISAnime in (0,1)) DEFAULT 1 NOT NULL
);
CREATE TABLE "Watched" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL DEFAULT NULL CHECK (id >=0),
"Series" TEXT NOT NULL DEFAULT NULL CHECK (Length(Series) >= 1),
"DateStarted" TEXT NOT NULL DEFAULT NULL CHECK (like('____-__-__ __:__:__',DateStarted)),
"DateFinished" TEXT NOT NULL DEFAULT NULL CHECK (like('____-__-__ __:__:__',DateFinished))
);
CREATE VIEW SeriesData as
SELECT 

	si.Title,
	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN si.RewatchingCurrent 
			ELSE si.Current 
		END
	) as Current, 
	
	si.Total,
	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN si.RewatchingDate 
			ELSE si.Date 
		END
	) as Date, 

	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN 0 
			ELSE si.Finished 
		END
	) as Finished, 

	(
		SELECT CASE WHEN 
			(Exists(Select * From OldSeries os Where os.Title=si.Title) AND si.Finished != 1 ) OR
			si.Rewatching = 1 
		THEN 1 ELSE 0 
		END
	) as Rewatching,
	
	si.Skip,
	si.dropped,
	si.Score,
	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN si.RewatchingStart
			ELSE si.StartDate 
		END
	) as StartDate,

	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN NULL
			ELSE si.EndDate 
		END
	) as EndDate,
	
	si.Updated,
	si.Id,
	(
		(Select COUNT(*) From Watched   Where Series = si.Title) +
		(Select COUNT(*) From OldSeries Where title  = si.Title)
	) as WatchCount,

	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN
				 si.RewatchingStart
			WHEN (Select COUNT(*) From Watched Where Series = si.Title) >= 1 THEN
				(Select DateStarted
				From Watched w
				Where w.Series = si.Title
				Order by w.DateStarted desc
				Limit 1)
			ELSE si.StartDate 
		END
	) as MostRecentStartDate,

	(
		SELECT CASE 
			WHEN si.Rewatching = 1 THEN
				 Null
			WHEN (Select COUNT(*) From Watched Where Series = si.Title) >= 1 THEN
				(Select w.DateFinished
				From Watched w
				Where w.Series = si.Title
				Order by w.DateFinished desc
				Limit 1)
			ELSE si.EndDate 
		END
	) as MostRecentEndDate,

	si.StartDate as FirstStartDate,
	si.EndDate as FirstEndDate


FROM SeriesInfo si
ORDER BY si.Title;
CREATE VIEW OnlyOldSeries As
	Select * from OldSeries os
	WHERE NOT (Select EXISTS (SELECT * FROM SeriesInfo WHERE Title = os.title))
ORDER by os.Title;
CREATE VIEW Recent As
SELECT 
	Title, Current, Total, 
	strftime("%Y-%m-%d %H:%M",date, 'localtime') as Date,
	Finished, Rewatching, Score
FROM SeriesData
WHERE
	( 
		strftime('%s',Date) > strftime('%s', 'now','-14 day','localtime') 
	)

Order By Date desc;
CREATE VIEW Rewatching As

Select 
	si.Title as Series,
	si.Current,
	si.Total,
	strftime("%Y-%m-%d %H:%M",si.Date,'localtime')      as Date,
	strftime("%Y-%m-%d %H:%M",si.StartDate, 'localtime')as StartDate

From SeriesData si

Where 
	si.rewatching = 1

Order by si.Title;
CREATE VIEW AllSeries As 

Select Title, Current, Total, Date, Finished, Score, Skip, Dropped, StartDate, EndDate, Updated, Id, 
	WatchCount, MostRecentStartDate, MostRecentEndDate, FirstStartDate, FirstEndDate, "SeriesData" as Type
From SeriesData sd

UNION

Select Title, Current, Total, Date, Finished, Score, Skip, Dropped, StartDate, EndDate, Updated, Id, 
	1, StartDate, EndDate, StartDate, EndDate, "OnlyOldSeries" as Type
From OnlyOldSeries oos

ORDER by Title;
CREATE VIEW DroppedSeries As

Select 
	si.Title as Series,
	si.Current,
	si.Total,
	strftime("%Y-%m-%d %H:%M",si.StartDate, 'localtime')as StartDate,
	strftime("%Y-%m-%d %H:%M",si.Date, 'localtime')as Date,
	si.Score
From SeriesInfo si

Where 
	si.Finished = 0 AND
	si.Dropped  = 1

Order by si.Title;
CREATE VIEW FinishedSeries As

Select 
si.Title as Series,
si.Total,
strftime("%Y-%m-%d %H:%M",si.StartDate, 'localtime')as StartDate,
strftime("%Y-%m-%d %H:%M",si.EndDate, 'localtime')as EndDate,
si.Score

From SeriesData si

Where si.Finished = 1
Order by si.Title;
CREATE VIEW OngoingSeries As

Select 
	si.Title as Series,
	si.Current,
	si.Total,
	strftime("%Y-%m-%d %H:%M",si.Date, 'localtime')     as Date,
	strftime("%Y-%m-%d %H:%M",si.StartDate, 'localtime')as StartDate,	si.Rewatching

From SeriesData si

Where 
	si.Finished   = 0 AND
	si.Dropped    = 0

Order by si.Title;

CREATE VIEW Scores as
Select Score, count(Score) as Count
From AllSeries
Where Score is NOT NULL
GROUP by Score;
CREATE VIEW ScoresByYear AS
Select d.Year, 

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 1
group by dd.Year
), 0) as 'N1',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 2
group by dd.Year
), 0) as 'N2',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 3
group by dd.Year
), 0) as 'N3',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 4
group by dd.Year
), 0) as 'N4',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 5
group by dd.Year
), 0) as 'N5',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 6
group by dd.Year
), 0) as 'N6',
IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 7
group by dd.Year
), 0) as 'N7',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 8
group by dd.Year
), 0) as 'N8',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 9
group by dd.Year
), 0) as 'N9',

IFNUll((
Select count(aa.Score)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and aa.Score = 10
group by dd.Year
), 0) as 'N10'

from AllSeries a 
Join MalData d  on a.Id = d.Id

group by Year
order by Year;
CREATE VIEW ToLink as 
Select * from SeriesInfo si
where si.Id not in (Select Id from MalData) and si.id not NUll
ORDER by Title;
CREATE VIEW TypesByYear as

Select d.Year, 

IFNUll((
Select count(dd.type)  as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and dd.Type = 'Movie'
group by dd.Type
), 0) as 'Movie',

IFNUll((
Select count(dd.type) as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and dd.Type = 'Music'
group by dd.Type
), 0) as 'Music',


IFNUll((
Select count(dd.type) as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and dd.Type = 'ONA'
group by dd.Type
),0) as 'ONA',

IFNUll((
Select count(dd.type) as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and dd.Type = 'OVA'
group by dd.Type
),0) as 'OVA',

IFNUll((
Select count(dd.type) as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and dd.Type = 'Special'
group by dd.Type
),0) as 'Special',

IFNUll((
Select count(dd.type) as Count
from  AllSeries aa Join MalData dd  on aa.Id = dd.Id
where dd.Year = d.year and dd.Type = 'TV'
group by dd.Type
),0) as 'TV'

from AllSeries a 
Join MalData d  on a.Id = d.Id

group by Year
order by Year;
CREATE VIEW WatchedSeriesDates As
Select 
	w.Series,
	si.Total,
	strftime("%Y-%m-%d %H:%M", w.DateStarted, 'localtime') as StartDate,
	strftime("%Y-%m-%d %H:%M", w.DateFinished, 'localtime') as EndDate,
	(strftime('%s',w.DateFinished) - strftime('%s', w.DateStarted))/60 as Minutes,
	round(julianday(w.DateFinished) - julianday(w.DateStarted)) as Days,
	round((julianday(w.DateFinished) - julianday(w.DateStarted)) /si.total,2) as 'DaysPerEp'

From Watched w Join
	SeriesInfo si On si.Title = w.Series

Order by Series;
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
		SET Rewatching    = 1,
		RewatchingStart   = NEW.Date,
		RewatchingDate    = NEW.Date,
		RewatchingCurrent = new.Number
		
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
CREATE TRIGGER OnSeriesFinishedOnUpdate AFTER UPDATE ON SeriesInfo WHEN
(
	EXISTS (SELECT Total FROM SeriesInfo WHERE ROWID = NEW.ROWID)       AND
	( (SELECT Rewatching FROM SeriesInfo WHERE ROWID = NEW.ROWID) = 0 ) AND
	( (SELECT Finished   FROM SeriesInfo WHERE ROWID = NEW.ROWID) = 1 ) AND
	NOT EXISTS (SELECT Series FROM Watched w
				WHERE w.Series       = (SELECT Title        FROM SeriesInfo WHERE ROWID = NEW.ROWID) AND 
					  w.DateFinished = (SELECT DateFinished FROM SeriesInfo WHERE ROWID = NEW.ROWID) AND
					  w.DateStarted  = (SELECT DateStarted  FROM SeriesInfo WHERE ROWID = NEW.ROWID) 
			   )
)
BEGIN
	INSERT INTO Watched(Series, DateStarted, DateFinished) 
		VALUES(
			(SELECT Title     FROM SeriesInfo WHERE ROWID = NEW.ROWID),
			(SELECT StartDate FROM SeriesInfo WHERE ROWID = NEW.ROWID), 
			(SELECT EndDate   FROM SeriesInfo WHERE ROWID = NEW.ROWID)
		);
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
CREATE TRIGGER ValidateEpNum2 BEFORE INSERT ON History WHEN
(
	EXISTS (SELECT * FROM SeriesInfo WHERE Title = NEW.Series) AND
	( (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series) != NULL) AND
	(NEW.Number > (SELECT Total FROM SeriesInfo WHERE Title = NEW.Series) )
)
BEGIN
	SELECT CASE
		When (Select 1 =1) then
		RAISE(ABORT,'Malformed Series title')
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