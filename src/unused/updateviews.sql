

CREATE TRIGGER UpdateAllSeries 
INSTEAD Of UPDATE ON AllSeries
BEGIN
SELECT CASE When
	(
		NEW.Type != "SeriesData" or
		Old.title != New.Title
	) Then
		RAISE ( ABORT, "Invaild" )	
	END;
	UPDATE SeriesInfo
	SET    Score = NEW.Score
	WHERE  Title = OLD.Title;
END