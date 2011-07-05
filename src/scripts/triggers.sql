
DROP TRIGGER A01_OnSeriesFinished;
DROP TRIGGER A11_OnSeriesRewatch;
DROP TRIGGER A22_SetUpdated;
DROP TRIGGER A41_UpdateSeriesInfoUpdatesOnRewatch;
DROP TRIGGER A45_OnSeriesRewatchFinish;

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