do
  -- The marker table serves double duty. It is a private key in the
  -- hash, and also serves as the hashes metatable.

  local marker = {}

  -- If the key is not found, we use the saved function (stored with the
  -- private key) to generate a value, and save it.

  function marker.__index(t, k)
    local val = t[marker](k)
    t[k] = val
    return val
  end

  -- We make the table look like a function, just deferring to lookup

  function marker.__call(t, k)
    return t[k]
  end

  -- They'll hit an endless loop if they do Memoize(nil). So we do
  -- something reasonable. We could also report an error, of course.

  function Memoize(fn)
    local self = {[marker] = fn or function(x) return nil end}
    setmetatable(self, marker)
    return self
  end

end
