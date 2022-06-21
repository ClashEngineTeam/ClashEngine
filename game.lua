title = "123"

x = 0
y = 0

buttonPosX = 500
buttonPosY = 400
buttonSizeX = 200
buttonSizeY = 100

r = 255
g = 0
b = 0

draw_red = true

function mouse_click(btn)
    if (draw_red == true) then
        draw_red = false
        r = 0
        g = 255
    else
        draw_red = true
        r = 255
        g = 0
    end
end

function draw_my_button(btn)
    buttonPos = get_ui_pos(btn)
    buttonSize = get_ui_size(btn)

    draw_image_scaling(buttonPos:x(), buttonPos:y(), button_image, buttonSize:x(), buttonSize:y())
    draw_frame(buttonPos:x(), buttonPos:y(), buttonSize:x(), buttonSize:y(), r, g, b)
end

function start()

    set_window_title(title)

    audio = init_audio("res/audios/sgs_background_music_default.mp3")
    qinghe = init_image("res/images/characters/QingHe2.png")
    hello_world = init_font("res/fonts/msyh.ttc", 72)

    --load button image:
    button_image = init_image("res/images/buttons/start.png")
    
    button = init_button(buttonPosX, buttonPosY, buttonSizeX, buttonSizeY)
    bind_mouse_click(button, "mouse_click")
    bind_draw(button, "draw_my_button")
    
    --disable_ui(button)
    newButton = init_button(buttonPosX + 10, buttonPosY + 10, 100, 100)
    set_ui_parent(newButton, button)
    
    pos = get_ui_local_pos(newButton)

    --Vector2:
    --position = Vector2.new(10, 10)
    --writeline_console(console, tostring(position:x()))

    test_font = render_string_to_sprite(hello_world, title, 0,0,0)
    font_sprite = render_string_to_sprite_ex(hello_world, title, 0, 0, 0, 50)

    inputField = init_input_field(400, 320, 400, 40, hello_world)

    video = init_video("res/videos/Yasuo.mp4")

    --local con = init_console()
    --console.write(con, "Hello World!")

    --open without chrome:
    --process_execute("https://www.youtube.com/")
    --open with chrome:
    --process_execute_ex("chrome", "youtube.com")

    console = init_console()
    processes = process_get_all()
    for k,v in ipairs(processes) do
        local processName = process_get_name(v)
        write_console(console, v)
        write_console(console, " ")
        writeline_console(console, processName)
        if processName == "League of Legends.exe" then
            local processHandle = process_get_handle(v)
            local processClosed = process_kill(processHandle)
            if processClosed then
                writeline_console(console, "League of Legends.exe closed")
            end
        end
        if processName == "FnHotkeys.exe" then
            local processHandle = process_get_handle(v)
            local processClosed = process_kill(processHandle)
            if processClosed then
                writeline_console(console, "FnHotkeys.exe closed")
            end
        end
    end

    --networking:
    enet_server = server.create(8848)
    writeline_console(console, "Server start!")

end

function update()

    --poll network event:
    local enet_event = server.poll_event(enet_server)
    local enet_event_type = server.get_event_type(enet_event)
    if enet_event_type ~= 0 then
        writeline_console(console, enet_event_type)
    end
    --release event:
    server.release_event(enet_event)

    clear_color(155,55,155)

    if GetKey(Key_A) then
        x = x - 1
    end
    if GetKey(Key_D) then
        x = x + 1
    end
    if GetKey(Key_W) then
        y = y - 1
    end
    if GetKey(Key_S) then
        y = y + 1
    end

    xx = get_mouse_x()
    yy = get_mouse_y()
    ww = get_mouse_wheel()

    --鼠标上滑
    if ww > 0 then
        y = y + 10
    end

    --鼠标下滑
    if ww < 0 then
        y = y - 10
    end
    
    if GetKeyDown(Key_F1) then
        console = init_console()
        write_console(console, "你好乐基")
    end

    if GetKeyDown(Key_F2) then
        deinit_console(console)
    end

    draw_image_scalingf(x, y, qinghe, 1, 1)

    dt = get_delta_time()
    
    mx = get_mouse_x()
    my = get_mouse_y()
    
    draw_ui(button)
    draw_ui(newButton)
    draw_ui(inputField)
    inputData = get_input_field_data(inputField)
    draw_string(hello_world, inputData, 300,300,255,255,255)

    draw_png_image(500, 200, font_sprite)

    local frameSprite = get_video_next_frame(video)
    if frameSprite ~= nil then
        draw_image_scaling(0, 0, frameSprite, 600, 300)
    end
end

function destroy()
    deinit_audio(audio)
    deinit_image(qinghe)
    --deinit_console(console)
end

init_engine(1200, 750)