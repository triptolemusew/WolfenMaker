extern crate sdl2;

mod constants;
mod graphics;
mod video;

use sdl2::{event::Event, keyboard::Keycode, pixels::Color};
use std::f32::consts::PI;
use video::Video;

const P2: f32 = PI / 2.;
const P3: f32 = 3. * PI / 2.;

#[rustfmt::skip]
const MAP: &[u8; 64] = &[
     1,1,1,1,1,1,1,1,
     1,0,1,0,0,0,0,1,
     1,0,1,0,0,0,0,1,
     1,0,1,0,0,0,0,1,
     1,0,0,0,0,0,0,1,
     1,0,0,0,0,1,0,1,
     1,0,0,0,0,0,0,1,
     1,1,1,1,1,1,1,1,
];

#[allow(non_upper_case_globals)]
pub const MapX: usize = 8;
pub const MapY: usize = 8;
pub const MapS: usize = 64;

fn draw_map_2d(video: &mut Video) {
    for y in 0..MapY {
        for x in 0..MapX {
            let color = match MAP[y * MapX + x] {
                1 => Color::RGB(0xff, 0xff, 0xff),
                _ => Color::RGB(0, 0, 0),
            };

            for big_y in 0..(MapS - 1) {
                for big_x in 0..(MapS - 1) {
                    video.put_pixel(
                        (x * MapS) as u32 + big_x as u32,
                        (y * MapS) as u32 + big_y as u32,
                        color,
                    );
                }
            }
        }
    }
}

// Player
fn draw_player_2d(video: &mut Video, px: f32, py: f32, pdx: f32, pdy: f32) {
    for y in 0..(MapS / 5) {
        for x in 0..(MapS / 5) {
            video.put_pixel(
                px as u32 + x as u32,
                py as u32 + y as u32,
                Color::RGB(0xff, 0xff, 0x00),
            );
        }
    }

    // Drawing the line ray for player
    let line_length = 30;
    for line in 0..line_length {
        let x_location = (px + (MapS / 10) as f32) + (line as f32 * pdx);
        let y_location = (py + (MapS / 10) as f32) + (line as f32 * pdy);
        video.put_pixel(
            x_location as u32,
            y_location as u32,
            // Color::RGB(0x00, 0x00, 0x00),
            Color::RGB(0xff, 0xff, 0x00),
        );
    }
}

// Rays
fn draw_rays_2d(video: &mut Video, px: f32, py: f32, pa: f32, pdx: f32, pdy: f32) {
    let mut ra = fix_ang(pa + 30.);
    let mut ry: f32 = 0.;
    let mut rx: f32 = 0.;
    let mut yo: f32 = 0.;
    let mut xo: f32 = 0.;
    let mut mx: u32 = 0;
    let mut my: u32 = 0;
    let mut mp: u32 = 0;

    let mut vx: f32 = 0.;
    let mut vy: f32 = 0.;

    for r in 0..60 {
        // Vertical lines
        let mut dof = 0;
        let mut side = 0;
        let mut disV = 100000.;
        let mut disH = 100000.;

        let mut tan: f32 = f32::tan(deg_to_rad(ra));

        let mut color_pixel: Color = Color::RGB(0x00, 0x66, 0x00);

        if f32::cos(deg_to_rad(ra)) > 0.001 {
            rx = (((px as u32) >> 6) << 6) as f32 + 64.;
            ry = (px - rx) * tan + py;
            xo = 64.;
            yo = -xo * tan;
        } else if f32::cos(deg_to_rad(ra)) < -0.001 {
            rx = (((px as u32) >> 6) << 6) as f32 - 0.0001;
            ry = (px - rx) * tan + py;
            xo = -64.;
            yo = -xo * tan;
        } else {
            rx = px;
            ry = py;
            dof = 8;
        }
        while dof < 8 {
            mx = ((rx as u32) >> 6);
            my = ((ry as u32) >> 6);
            mp = my * (MapX) as u32 + mx;
            if mp > 0 && mp < (MapX * MapY) as u32 && MAP[mp as usize] == 1 {
                dof = 8;
                disV = f32::cos(deg_to_rad(ra)) * (rx - px) - f32::sin(deg_to_rad(ra)) * (ry - py);
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        vx = rx;
        vy = ry;

        // Horizontal lines
        dof = 0;
        tan = 1.0 / tan;

        if f32::sin(deg_to_rad(ra)) > 0.001 {
            ry = (((py as u32) >> 6) << 6) as f32 - 0.0001;
            rx = (py - ry) * tan + px;
            yo = -64.;
            xo = -yo * tan;
        } else if f32::sin(deg_to_rad(ra)) < -0.001 {
            ry = (((py as u32) >> 6) << 6) as f32 + 64.;
            rx = (py - ry) * tan + px;
            yo = 64.;
            xo = -yo * tan;
        } else {
            rx = px;
            ry = py;
            dof = 8;
        }

        while dof < 8 {
            mx = ((rx as u32) >> 6);
            my = ((ry as u32) >> 6);
            mp = my * (MapX) as u32 + mx;
            if mp > 0 && mp < (MapX * MapY) as u32 && MAP[mp as usize] == 1 {
                dof = 8;
                disH = f32::cos(deg_to_rad(ra)) * (rx - px) - f32::sin(deg_to_rad(ra)) * (ry - py);
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        // Check which hit firstl, horizontal or vertical
        if disV < disH {
            rx = vx;
            ry = vy;
            disH = disV;
            color_pixel = Color::RGB(0x00, 0x66, 0x00);
        }
        if disH < disV {
            color_pixel = Color::RGB(0x00, 0xff, 0x00);
        }
        // Using pythagoras bic,
        let distance = f32::sqrt(f32::powf(rx - px, 2.) + f32::powf(ry - py, 2.));

        for line in 0..(distance as u32) {
            // NOTE: Below would've worked if for single ray. due to the pdx, pdy.
            // let x_location = (px as f32) + (line as f32 * pdx);
            // let y_location = (py as f32) + (line as f32 * pdy);

            let x_location = (px as f32) + (line as f32 * f32::cos(deg_to_rad(ra)));
            let y_location = (py as f32) + (line as f32 * -f32::sin(deg_to_rad(ra)));

            video.put_pixel(
                x_location as u32,
                y_location as u32,
                Color::RGB(0xff, 0x00, 0x00),
            );
        }

        let ca = fix_ang(pa - ra);
        disH = disH * f32::cos(deg_to_rad(ca));
        let mut lineH: u32 = (MapS * 320) as u32 / (disH as u32);

        if lineH > 320 {
            lineH = 320;
        }
        let lineOff = 160 - (lineH >> 1);
        // Draw the actual '3d' graphics

        for line in 0..lineH {
            for extra in 0..8 {
                let x = (r * 8 + 530) + extra;
                let y = lineOff + line;
                video.put_pixel(x, y, color_pixel);
            }
        }

        ra = fix_ang(ra - 1.);

        println!(
            "r: {} ra: {} distance: {} pdx: {} pdy: {} rx: {} ry: {}",
            r, ra, distance, pdx, pdy, rx, ry
        );
    }
}

fn deg_to_rad(a: f32) -> f32 {
    return (a * PI) / 180.;
}

fn fix_ang(mut a: f32) -> f32 {
    if a > 359. {
        a -= 360.;
    }
    if a < 0. {
        a += 360.;
    }
    a
}

fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem
        .window("wolfen", 1200, 800)
        .position_centered()
        .build()
        .map_err(|e| e.to_string())
        .unwrap();

    let mut canvas = window
        .into_canvas()
        .accelerated()
        .build()
        .map_err(|e| e.to_string())
        .unwrap();

    let texture_creator = canvas.texture_creator();
    let mut events = sdl_context.event_pump().unwrap();

    let mut video = Video::new(1, &texture_creator);

    let mut px = 300.;
    let mut py = 300.;
    let mut pa = 90.;
    let mut pdx = f32::cos(deg_to_rad(pa));
    let mut pdy = -f32::sin(deg_to_rad(pa));

    // Main loop for the wolfenmaker
    'running: loop {
        video.clear_buffer();

        draw_map_2d(&mut video);
        draw_player_2d(&mut video, px, py, pdx, pdy);
        draw_rays_2d(&mut video, px, py, pa, pdx, pdy);

        video.present(&mut canvas);

        for event in events.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,
                Event::KeyDown { keycode, .. } => {
                    if let Some(key) = keycode {
                        match key {
                            Keycode::Up => {
                                // // py -= 10.;
                                // px += pdx * 5.;
                                // py += pdy * 5.;
                                px += pdx * 5.;
                                py += pdy * 5.;
                            }
                            Keycode::Down => {
                                // // py += 10.;
                                // px -= pdx * 5.;
                                // py -= pdy * 5.;
                                px -= pdx * 5.;
                                py -= pdy * 5.;
                            }
                            Keycode::Left => {
                                pa += 5.;
                                pa = fix_ang(pa);
                                pdx = f32::cos(deg_to_rad(pa));
                                pdy = -f32::sin(deg_to_rad(pa));
                            }
                            Keycode::Right => {
                                pa -= 5.;
                                pa = fix_ang(pa);
                                pdx = f32::cos(deg_to_rad(pa));
                                pdy = -f32::sin(deg_to_rad(pa));
                            }
                            _ => {}
                        }
                    }
                }
                _ => {}
            }
        }
    }
}
