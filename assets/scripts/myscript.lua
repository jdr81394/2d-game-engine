-- This is a Lua global variable
some_variable = 7 * 6
user_name = "Gustavo"

if some_variable == 42 then
    print("The value of the variable is " ..some_variable)
end

-- This is a lua table that contains values for the initialization of our game engine
config = {
    title = "My Game Engine",
    fullscreen = false,
    resolution = {
        width = 800,
        height = 600
    }
}

function factorial(n)
    -- TODO: Create body of this function
    ret_value = 1
    for i=1, n,1  do
        ret_value = ret_value * i
    end

    return ret_value;
end


function callCube(n) 
    return_value = cube(n);
    print("this is the call cube value: " ..return_value)
end

-- factorial(3)
-- factorial(5)
-- factorial(7)
callCube(10);