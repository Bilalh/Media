#!/usr/bin/env ruby19 -KU
# require "rubygems"
require 'sqlite3'
require 'nokogiri'
require 'open-uri'
require 'json'
require 'net/http'
require 'htmlentities'
require "yaml"

#  Convert from under_scores to CamelCase
class String
	
	def inter_camelcase
	  str = dup
	  str.gsub!(/\/(.?)/){ "::#{$1.upcase}" }  # not so sure about this
	  str.gsub!(/(?:_+|-+)([a-z])/){ $1.upcase }
	  str
	end

	def upper_camelcase
	  inter_camelcase.gsub(/(\A|\s)([a-z])/){ $1 + $2.upcase }
	end
	
end


def check(value)
	if !value || value == "" || value == [] || value == {} then "NULL" else value  end
end

def mal_search(id)
   url = "http://mal-api.com/anime/#{id}"
   resp = Net::HTTP.get_response(URI.parse(url))
   data = resp.body

   # we convert the returned JSON data to native Ruby
   result = JSON.parse(data)

   # if the hash has 'Error' as a key, we raise an error
   if result.has_key? 'error'
      raise "invalid id #{id} - #{result['error']}"
   end
   return result
end

def make_table(_keys)
	keys =  _keys.map{ |x| x.upper_camelcase }
	fields = keys.map { |e| "\"#{e}\" TEXT" }.join ",\n"
	query = <<-SQL
	CREATE TABLE "MalData" (
	#{fields}
	);
	SQL
end

def get_data(db)
	coder = HTMLEntities.new
	ids = []
	db.results_as_hash = true;
	db.execute( 'Select * from AllSeries where Id Is Not NULL').each do |row|
		ids << {"id" =>row["Id"], "realTitle" =>row["Title"]};
	end

	ids.each do |row|
		puts "Getting #{row["id"]}"
	
		data = mal_search(row["id"])
		# data = mal_search(4472)
		# data = mal_search(444)
		
		xml = `curl -u bhterra:bhterramai\#  "http://myanimelist.net/api/anime/search.xml?q=#{data["title"]}"`
		doc = Nokogiri::HTML(xml)
		date                 = doc.css("anime start_date")[0].text
		data["year"]         = date[/(\d{4}).*/,1]
		data["startAirDate"] = doc.css("anime start_date")[0].text
		data["endAirDate"]   = doc.css("anime end_date")[0].text
	
		# puts data
		data["english"]      = data["other_titles"]["english"][0] if data["other_titles"]["english"]
		data["japanese"]     = data["other_titles"]["japanese"][0] if data["other_titles"]["japanese"]
	
		#  decode html
		["english","japanese","title"].each do |e|
			data[e] = coder.decode data[e]  if  data[e]
		end
	
		row.merge! data
		puts "Got #{row["id"]} #{row["title"]}\n"
	end
	return ids
end

TagQuery = <<-SQL
	Insert Into MalTags(SeriesId, Tag)
	Values(?, ?)
SQL

GenreQuery = <<-SQL
	Insert Into MalGenres(SeriesId, Genre)
	Values(?, ?)
SQL

RelationQuery = <<-SQL
	Insert Into MalRelation(SeriesId, RelationType, RelationId, RelationUrl, RelationTitle)
	Values(?, ?, ?, ?, ?)
SQL

SynonymQuery = <<-SQL
	Insert Into MalSynonyms(SeriesId, Synonym)
	Values(?, ?)
SQL

def update_database(db,ids)
	coder = HTMLEntities.new

	extra     = ["tags", "genres" ]
	relations = ["sequels", "prequels", "side_stories"]
	other     = ["other_titles", "manga_adaptations"]
	useless   = ["watched_episodes", "watched_status", "score", "realTitle"]


	ids.each do |h|
		puts "Processing #{h["id"]} #{h["title"]} -- #{h["realTitle"]}"
		_keys  = h.keys - extra - relations - other - useless;
		# puts make_table _keys ;exit

		keys   = _keys.map{ |x| x.upper_camelcase }
		values = _keys.map{ |key| h[key] }

		qs=(['?'] * values.length).join ',';
		query = <<-SQL
		Insert Into MalData(#{keys.join ","})
		Values( #{qs})
		SQL

		db.execute(query, values.map { |e| check e } )


		tableQuery = ->(field,query,hash=h) do
			if hash[field] then
				hash[field].each do |value|
					db.execute(query, h["id"], coder.decode(value))
				end
			end
		end

		tableQuery["tags", TagQuery]
		tableQuery["genres", GenreQuery]
		tableQuery["synonyms",SynonymQuery,h["other_titles"]]
		tableQuery["english",SynonymQuery,h["other_titles"]]
		tableQuery["japanese",SynonymQuery,h["other_titles"]]

	
		relationsQuery = ->(field, id="anime_id") do
			if check h[field] then
				h[field].each do |hash|
					db.execute(RelationQuery, h["id"], field, 
						hash[id],hash["url"], coder.decode(hash["title"]) )
				end
			end
		end

		relations.each { |field| relationsQuery[field] }
		relationsQuery["manga_adaptations", "manga_id"] 
	end
end


db    = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )
# ids   = get_data(db)
# open('mal.yaml', 'w') { |f| YAML.dump(ids, f) }
ids = {}
open('mal.yaml', 'r') { |f| ids = YAML.load(f) }
update_database(db, ids)
