-- usage sqlite3 media.db < link_media_to_ids.sql 
ATTACH DATABASE "mal.db" AS Maldb;
Create View temp.vIds as Select Distinct Id from Maldb.MalData where Id in (Select Id From ToLink);
Create Table temp.Ids (Id Integer);
Insert into  temp.Ids([Id]) Select Distinct  [Id] from temp.vIds;

Insert into MalData([Id], [Status],[Type],[ListedAnimeId],[Classification],[MembersScore],[MembersCount],[Rank],[FavoritedCount],[Title],[PopularityRank],[Episodes],[Synopsis],[Year],[StartAirDate],[EndAirDate],[English],[Japanese]) 
Select[Id], [Status],[Type],[ListedAnimeId],[Classification],[MembersScore],[MembersCount],[Rank],[FavoritedCount],[Title],[PopularityRank],[Episodes],[Synopsis],[Year],[StartAirDate],[EndAirDate],[English],[Japanese] from Maldb.MalData mm 
where mm.Id in (Select Id from temp.Ids);

Insert into MalGenres([SeriesId],[Genre]) 
Select [SeriesId], [Genre] from Maldb.MalGenres mm 
where mm.SeriesId in (Select Id from temp.Ids);

Insert into MalTags([SeriesId],[Tag])
Select [SeriesId], [Tag] from Maldb.MalTags mm 
where mm.SeriesId in (Select Id from temp.Ids);

Insert into MalSynonyms([SeriesId],[Synonym])
Select [SeriesId], [Synonym] from Maldb.MalSynonyms mm 
where mm.SeriesId in (Select Id from temp.Ids);

Insert into MalRelation([SeriesId],[RelationType],[RelationId],[RelationUrl],[RelationTitle])
Select [SeriesId],[RelationType],[RelationId],[RelationUrl],[RelationTitle] from Maldb.MalRelation mm 
where mm.SeriesId in (Select Id from temp.Ids);

