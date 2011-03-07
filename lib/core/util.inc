<?php

namespace Craft\Util;

/**
 * Useful functions.
 */

/**
 * recursiveCopy
 * Copies files/folders recursively, making folders if necessary.
 * @param string $source
 * @param string $dest
 */

function recursiveCopy($source, $dest)
{
    // Check for symlinks
    if (is_link($source)) {
        return symlink(readlink($source), $dest);
    }
 
    // Simple copy for a file
    if (is_file($source)) {
        return copy($source, $dest);
    }
 
    // Make destination directory
    if (!is_dir($dest)) {
        mkdir($dest);
    }
 
    // Loop through the folder
    $dir = dir($source);
    while (false !== $entry = $dir->read()) {
        // Skip pointers
        if ($entry == '.' || $entry == '..') {
            continue;
        }
 
        // Deep copy directories
        \anApp\functions\recursiveCopy("$source/$entry", "$dest/$entry");
    }
 
    // Clean up
    $dir->close();
    return true;
}


?>