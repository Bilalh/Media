db="/Users/bilalh/Library/Application Support/Media/Media.db"
sqlite3 "${db}" 'select total,score from  FinishedSeries fs order by fs.Total;' | sed 's/|/ /'