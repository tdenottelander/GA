GPU=$1
TIME=$2

while true
do
  current_time=$(date "+%Y.%m.%d-%H.%M.%S")
  echo ""
  echo "Trying to process encoding at $current_time"

  if [ -f queue/encodings.json ]
  then
    echo "New batch of encodings available"
    # Rename the encodingsfile
    mv queue/encodings.json queue/encodings_gpu$GPU.json

    # Run python script here to evaluate the encoding
    python3 nas_worker.py queue/encodings_gpu$GPU.json

    # When done, create 'done file'
    touch queue/done.json
  else
    # Create done.json if for some reason it is deleted for some other worker
    if [ ! -f queue/done.json ]
    then
      touch queue/done.json
    fi
    echo "No encoding available right now. Sleeping for $TIME seconds."
    echo ""
    sleep $TIME
  fi
done
