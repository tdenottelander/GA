while true
do
  current_time=$(date "+%Y.%m.%d-%H.%M.%S")
  echo ""
  echo "Trying to process encoding at $current_time"

  if [ -f queue/encoding.json ]
  then
    echo "New encoding available"

    # Run python script here to evaluate the encoding
    python3 nas_worker.py

    # When done, create 'done file'
    touch queue/done.json
    rm queue/encoding.json
  else
    # Create done.json if for some reason it is deleted for some other worker
    if [ ! -f queue/done.json ]
    then 
      touch queue/done.json
    fi
    echo "No encoding available right now. Sleeping for $1 seconds."
    echo ""
    sleep $1
  fi
done
