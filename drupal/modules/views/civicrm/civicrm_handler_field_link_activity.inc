<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 3.1                                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/
/*
 * Copyright (C) 2009 James Rakich
 * Licensed to CiviCRM under the Academic Free License version 3.0.
 *
 *
 * civicrm_handler_field_link_activity.inc
 * Completely customizable text that can be linked to numerous places related to the activity.
 * View, Edit and Delete Pages, as well as a custom Drupal Link with the activity ID appended,
 * which is great for linking to other views using the activity base table.
 *
 */

/**
 * Field handler to provide simple renderer that allows linking to various pages involving the activity.
 */
class civicrm_handler_field_link_activity extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
    $this->additional_fields['source_contact_id'] = 'source_contact_id';
    $this->additional_fields['activity_type_id'] = 'activity_type_id';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_civicrm_activity'] = array('default' => 'view');
    $options['custom_link'] = array('default' => '');
    $options['link_text'] = array('default' => '');
    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm_activity'] = array(
      '#type' => 'select',
      '#title' => t('Choose where to link this field'),
      '#options' => array(
         'view' => t('Link to View Activity'),
         'edit' => t('Link to Edit Activity'),
         'delete' => t('Link to Delete Activity'),
         'custom' => t('Link to a Custom Node'),
         ),
      '#default_value' => !empty($this->options['link_to_civicrm_activity']),
    );
    
    $form['custom_link'] = array(
       '#type' => 'textfield',
       '#title' => t('Custom Link to a Drupal Node'),
       '#description' => t('Link to a Drupal Node or View, from the base path. The Activity ID will be appended to the end of the link.'),
       '#default_value' => isset($this->options['custom_link']) ? $this->options['custom_link'] : '',
       '#dependency' => array('link_to_civicrm_activity' => array('custom')),
     );
     
     $form['link_text'] = array(
       '#type' => 'textfield',
       '#title' => t('Links Text'),
       '#description' => t('The text that will display in the field with the link.'),
       '#default_value' => isset($this->options['link_text']) ? $this->options['link_text'] : '',
     );

  }


  function render_link($data, $values) {
    
    $link_text = $this->options['link_text'];
    $link_choice = $this->options['link_to_civicrm_activity'];
    
    if ($link_choice == 'custom') {
       $custom_link = $this->options['custom_link'];
     }


    switch ($link_choice) {
      
    // LINKING TO activity VIEW PAGE
      case 'view':
           if (user_access('view all activities') && $link_text !== NULL && $link_text !== '') {
           return civicrm_views_href( $link_text,
                                            'civicrm/contact/view/activity',
                                            "atype={$values->civicrm_activity_activity_type_id}&action=view&reset=1&id={$values->id}&cid={$values->civicrm_activity_source_contact_id}&context=home" );
           }

    // LINKING TO activity EDIT PAGE
      case 'edit':
           if (user_access('view all activities') && $link_text !== NULL && $link_text !== '') {
           return civicrm_views_href( $link_text,
                                            'civicrm/contact/view/activity',
                                            "atype={$values->civicrm_activity_activity_type_id}&action=update&reset=1&id={$values->id}&cid={$values->civicrm_activity_source_contact_id}&context=home" );
           }

    // LINKING TO activity DELETE PAGE
      case 'delete':
           if (user_access('view all activities') && $link_text !== NULL && $link_text !== '') {
           return civicrm_views_href( $link_text,
                                            'civicrm/contact/view/activity',
                                            "atype={$values->civicrm_activity_activity_type_id}&action=delete&reset=1&id={$values->id}&cid={$values->civicrm_activity_source_contact_id}&context=home" );
           }                                                                                                                                    
           
    // CUSTOM NODE LINKAGE, GOOD FOR LINKING TO OTHER VIEWS WITH AN ARGUMENT
      case 'custom':
           if ($custom_link !== NULL && $custom_link !== '' && $link_text !== NULL && $link_text !== '') {
           return l($link_text, $custom_link . "{$values->id}" );
           }
    
    // IF THE OTHER CASES AREN'T IN PLAY, THEN JUST PRINT THE TEXT
      default:
              return $link_text;



    }
  }
  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}
