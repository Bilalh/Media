#!/bin/bash
$HOME/bin/ongoing | grep -P "${1}\s+(?=\d+/[\d?])" | grep -Po '\d+(?=/[\d?])'
