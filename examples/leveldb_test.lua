package.cpath = "luaclib/?.so"

local leveldb = require 'leveldb'

opt = leveldb.options()
opt.createIfMissing = true
opt.errorIfExists = false
opt.writeBufferSize = 64 * 1024 * 1024
local db = leveldb.open(opt, 'E:/test.db')

local st = os.clock()
local batch = leveldb.batch()
local writOpts = leveldb.writeOptions()
writOpts.sync = true
for i = 1, 5000000, 1 do
	batch:put(i, i)
	if i % 10000 == 0 then
		db:write(batch, writOpts)
		batch:clear()
		print(i)
	end
end
db:write(batch, writOpts)
leveldb.close(db)
local et = os.clock()

io.read()
