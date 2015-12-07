local skynet = require "skynet"
local dns = require "dns"

skynet.start( function()
	print("nameserver:", dns.server("192.168.1.1", 53))
	-- set nameserver
	-- you can specify the server like dns.server("8.8.4.4", 53)
	local address = "www.sina.com.cn"
	local ip, ips = dns.resolve(address)
	for k, v in ipairs(ips) do
		print(address, v)
	end
end )
