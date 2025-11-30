Just putting something down in text... will revisit later.

Side note - this website seems to have a good list of floor plan symbols [bigrentz | Floor Plan Symbols, Abbreviations, and Meanings](https://www.bigrentz.com/blog/floor-plan-symbols)

In Floor Plan Markdown, you create your floor plan by specifying
walls and things along those walls, such as doors and windows.

The content is organized into blocks like so:

```
<item> <flavor>
  <item data>
```

Currently the only supported item is the `wall`.


# Walls

The wall item block is structured like this:

```
wall <flavor>
  x1 y1
  x2 y2
  <sub-structure>
  x3 y3
  ...
```

`flavor` changes the line style of the wall. It is optional, with possible values:

| Flavor   | Description                 |
|----------|-----------------------------|
| interior | Default styling.            |
| exterior | Thicker line than interior. |

The main item data for walls are the `x y`'s. These specify the path of the wall. `0 0` is the top
left of the diagram, with `x` increasing to the right and `y` increasing to the bottom.

Along the wall path, you can specify `sub-structures`. These are things that are part of the wall but
interrupt the path - like doors and windows.

## Wall sub-structures

### window

```
window <distance> <length>
```

| Argument | Description                                                                    |
|----------|--------------------------------------------------------------------------------|
| distance | Specifies how far is the window from the previous `x y` point on the wall path |
| length   | Specifies how wide the window is                                               |

### door

```
door <distance> <length> <flavor> <direction> <orientation>
```

| Argument    | Description                                                                      |
|-------------|----------------------------------------------------------------------------------|
| distance    | Specifies how far is the door from the previous `x y` point on the wall path     |
| length      | Specifies how wide the door is                                                   |
| flavor      | Determines the style of door. Refer to the list below for supported values.      |
| direction   | Acceptable values are `in` or `out`. Check below if needed for your `flavor`     |
| orientation | Acceptable values are `left` or `right`. Check below if needed for your `flavor` |


Flavors

| Flavor         | direction required | orientation required |
|----------------|--------------------|----------------------|
| bifold         | yes                | no                   |
| closet         | yes                | yes                  |
| single         | yes                | yes                  |
| open           | no                 | no                   |
