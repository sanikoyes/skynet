require "signal"

signal.signal("SIGTERM", function() io.stdout:write("bye!\n") os.exit(-1) end);

signal.raise("SIGTERM");
