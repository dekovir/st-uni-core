// API
declare function log(text: string): void;

declare function random(): i32
declare function random_f(): f32

declare function screen_x(): u32;
declare function screen_y(): u32;

declare function mouse_x(): i32;
declare function mouse_y(): i32;
declare function mouse_button(): boolean;

declare function sprite_draw_at(x: f32, y: f32): void;

class Entity {
  x: f32
  y: f32
  vx: f32
  vy: f32

  constructor(x: f32, y: f32) {
    this.x = x;
    this.y = y;
    this.vx = 0;
    this.vy = 0;
  }
}

let entities = new Array<Entity>();

function spawn_entity(x: f32, y: f32): void
{
  let entity = new Entity(x, y);
  entity.vx = 150 * (2 * random_f() - 1)
  entity.vy = 150 * (2 * random_f() - 1)
  entities.push(entity);
}

function spawn_entities(count: i32): void
{
  let s_x = <f32>screen_x();
  let s_y = <f32>screen_y();

  for (let i = 0; i < count; i++)
  {
    let x = s_x * random_f();
    let y = s_y * random_f();
    spawn_entity(x, y);
  }
}

export function app_init(): void
{
  log("init called");
  spawn_entities(100);
}

export function app_update(delta: f32): void
{
  if (mouse_button())
    spawn_entity(<f32>mouse_x(), <f32>mouse_y());

  let s_x = <f32>screen_x();
  let s_y = <f32>screen_y();

  for (let i = 0; i < entities.length; i++) {
    let entity = entities[i];

    entity.x += entity.vx * delta;
    entity.y += entity.vy * delta;

    if (entity.x <= 0 || entity.x >= s_x)
      entity.vx *= -1;

    if (entity.y <= 0 || entity.y >= s_y)
      entity.vy *= -1;
  }
}

export function app_draw(): void
{
  for (let i = 0; i < entities.length; i++) {
    let entity = entities[i];
    sprite_draw_at(entity.x, entity.y);
  }
}