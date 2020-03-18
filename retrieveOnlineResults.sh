for i in 2 3 4 5
do
  rsync -vamr --include='**/*run*seed*/*.json' --include='*/' --exclude='*' tdo@meteor0$i:/export/scratch1/home/shared/GA/data/ ./data/online
done

minutes=20
FILES=data/online/*/MO_info0.json
for f in $FILES
do
  # echo "Processing $f file..."
  modificationTime=$(stat -f "%m%t" $f)
  now=$(date +%s)
  difference=$(((now-modificationTime)/60))

  if [ $difference -gt $minutes ]
  then
    echo "$f: Last updated: $difference minutes ago. (Check if OK!)"
  else
    echo "$f: Last updated: $difference minutes ago."
  fi
done
